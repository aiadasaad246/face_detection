FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive
ENV PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
ENV CMAKE_PREFIX_PATH=/usr/local

# Step 1: Install dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    meson \
    ninja-build \
    pkg-config \
    git \
    cmake \
    ca-certificates \
    libglib2.0-dev \
    gstreamer1.0-tools \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    gstreamer1.0-libav \
    libgstreamer1.0-dev \
    libgstreamer-plugins-base1.0-dev \
    libgstreamer-plugins-good1.0-dev \
    wget \
    unzip \
    && rm -rf /var/lib/apt/lists/*

# Step 2: Build and install OpenCV from source
WORKDIR /opt
RUN git clone --branch 4.8.0 --depth 1 https://github.com/opencv/opencv.git && \
    git clone --branch 4.8.0 --depth 1 https://github.com/opencv/opencv_contrib.git && \
    mkdir -p opencv/build && cd opencv/build && \
    cmake -D CMAKE_BUILD_TYPE=Release \
          -D CMAKE_INSTALL_PREFIX=/usr/local \
          -D OPENCV_EXTRA_MODULES_PATH=/opt/opencv_contrib/modules \
          -D BUILD_LIST=core,imgproc,objdetect \
          -D BUILD_EXAMPLES=OFF \
          .. && \
    make -j"$(nproc)" && \
    make install && \
    ldconfig

# Step 3: Create opencv4.pc for pkg-config
RUN mkdir -p /usr/local/lib/pkgconfig && \
    printf "prefix=/usr/local\nexec_prefix=\${prefix}\nlibdir=\${exec_prefix}/lib\nincludedir=\${prefix}/include/opencv4\n\nName: OpenCV\nDescription: Open Source Computer Vision Library\nVersion: 4.8.0\nLibs: -L\${libdir} -lopencv_core -lopencv_imgproc -lopencv_objdetect\nCflags: -I\${includedir}\n" > /usr/local/lib/pkgconfig/opencv4.pc

# Step 3.5: Create symlink for Haar cascades to expected location
RUN mkdir -p /usr/share && \
    ln -s /usr/local/share/opencv4 /usr/share/opencv4

# Step 4: Copy your plugin source code into the container
WORKDIR /app
COPY . .

# ✅ Diagnostic step before building
RUN echo "--- Checking OpenCV pkg-config output ---" && \
    pkg-config --cflags opencv4 && \
    pkg-config --libs opencv4 && \
    echo "--- End OpenCV check ---"

# Step 5: Build your plugin
RUN rm -rf build && \
    meson setup build --wipe && \
    ninja -C build && \
    ninja -C build install

# Step 6: Set plugin environment and default command
ENV GST_PLUGIN_PATH=/usr/local/lib/x86_64-linux-gnu/gstreamer-1.0
CMD ["gst-inspect-1.0", "finalplugin"]
