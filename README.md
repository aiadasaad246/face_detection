# Face Blur GStreamer Plugin

## Project Level

**Level Chosen:3

## Overview

This project implements a custom GStreamer plugin that detects and blurs faces in video streams using OpenCV's Haar Cascade classifier. The plugin is containerized using Docker for easy deployment and distribution.

## Build Instructions

### Docker Build Command
```bash
docker build -t finalplugin .
```

### Docker Run Commands

#### Inspect the Plugin
```bash
docker run --rm finalplugin:latest
```

#### Process a Video File
```bash
# Create output directory first
mkdir -p output

# Process your video file (replace 'your_video.mp4' with your actual video file)
docker run --rm -v $(pwd):/input -v $(pwd)/output:/output finalplugin:latest \
  gst-launch-1.0 filesrc location=/input/fa.mp4 ! decodebin ! videoconvert ! finalplugin ! \
  videoconvert ! avenc_mpeg4 ! avimux ! filesink location=/output/blurred_output.avi
```

#### Test with Generated Video Pattern
```bash
# Create output directory first
mkdir -p output

# Generate and process test video
docker run --rm -v $(pwd)/output:/output finalplugin:latest \
  gst-launch-1.0 videotestsrc num-buffers=300 ! videoconvert ! finalplugin ! \
  videoconvert ! avenc_mpeg4 ! avimux ! filesink location=/output/test_output.avi
```

## Features

- **Face Detection:** Uses OpenCV Haar Cascade classifier for real-time face detection
- **Face Blurring:** Applies Gaussian blur to detected face regions
- **GStreamer Integration:** Implemented as a native GStreamer plugin
- **Containerized:** Fully containerized for easy deployment and distribution
- **Video Processing:** Supports various video formats through GStreamer's decodebin

## Technical Details

### Plugin Information
- **Plugin Name:** finalplugin
- **Element Name:** finalplugin
- **Type:** Video Filter/Effect
- **Input/Output Format:** BGR video format
- **Dependencies:** GStreamer 1.24.2, OpenCV 4.8.0

### Architecture
- **Base Image:** Ubuntu 24.04
- **Build System:** Meson + Ninja
- **OpenCV:** Built from source (4.8.0) with core, imgproc, and objdetect modules
- **GStreamer:** Uses GstVideoFilter base class for video processing

## Requirements

- Docker or Podman
- Input video files in standard formats (MP4, AVI, MOV, etc.)
- Sufficient disk space for processing output

## Usage Examples

### Processing Your Own Video
1. Place your video file (e.g., `fa.mp4`) in the project directory
2. Run the processing command:
```bash
docker run --rm -v $(pwd):/input -v $(pwd)/output:/output finalplugin:latest \
  gst-launch-1.0 filesrc location=/input/fa.mp4 ! decodebin ! videoconvert ! finalplugin ! \
  videoconvert ! avenc_mpeg4 ! avimux ! filesink location=/output/fa_blurred.avi
```

### Performance Testing
```bash
# Test processing speed with verbose output
docker run --rm finalplugin:latest \
  gst-launch-1.0 -v videotestsrc num-buffers=100 ! videoconvert ! finalplugin ! fakesink
```

## Output

The processed video will be saved in the `output` directory with faces automatically detected and blurred. The plugin maintains the original video resolution and framerate while applying face anonymization.

## Troubleshooting

- **"No such file or directory" error:** Ensure the output directory exists (`mkdir -p output`)
- **"No element" errors:** The Docker image includes all necessary GStreamer plugins
- **Large file sizes:** Output videos use MPEG4 compression, but processing time depends on input video length and resolution
- **No faces detected:** The plugin uses Haar Cascade which works best with frontal faces in good lighting

## Project Structure

```
.
├── Dockerfile              # Container build instructions
├── README.md              # This file
├── meson.build           # Build configuration
├── source/
│   └── code/
│       └── c.cpp         # Plugin source code
└── output/               # Generated output directory
```
