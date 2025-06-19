plugin_name = 'finalplugin'

sources = [
  'c.cpp',
]

gstreamer_plugins = shared_library(plugin_name,
  sources,
  include_directories: include_directories('.'),
  dependencies: [gst_dep, gstvideo_dep, opencv_dep],
  install: true,
  install_dir: join_paths(get_option('libdir'), 'gstreamer-1.0')
)

