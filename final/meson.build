project('final-plugin', 'cpp',
  version: '1.0',
  default_options: ['warning_level=2', 'cpp_std=c++17']
)

gst_dep = dependency('gstreamer-1.0')
gstvideo_dep = dependency('gstreamer-video-1.0')

# Remove method: 'cmake' and let it use pkg-config
opencv_dep = dependency('opencv4', modules: ['core', 'imgproc', 'objdetect'])

subdir('source/code')

