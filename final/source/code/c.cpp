// c.cpp
#define PACKAGE "finalplugin"
#include "c.h"

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/video/gstvideofilter.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

GST_DEBUG_CATEGORY_STATIC(gst_finalplugin_debug);
#define GST_CAT_DEFAULT gst_finalplugin_debug

G_DEFINE_TYPE(GstFinalPlugin, gst_finalplugin, GST_TYPE_VIDEO_FILTER)

// Pad templates (static caps in/out)
static GstStaticPadTemplate sink_template = GST_STATIC_PAD_TEMPLATE(
    "sink", GST_PAD_SINK, GST_PAD_ALWAYS,
    GST_STATIC_CAPS("video/x-raw, format=(string)BGR")
);

static GstStaticPadTemplate src_template = GST_STATIC_PAD_TEMPLATE(
    "src", GST_PAD_SRC, GST_PAD_ALWAYS,
    GST_STATIC_CAPS("video/x-raw, format=(string)BGR")
);

static GstFlowReturn gst_finalplugin_transform_frame_ip(GstVideoFilter *filter, GstVideoFrame *frame)
{
    GstFinalPlugin *self = (GstFinalPlugin *)filter;

    cv::Mat img(cv::Size(GST_VIDEO_FRAME_WIDTH(frame), GST_VIDEO_FRAME_HEIGHT(frame)),
                CV_8UC3, GST_VIDEO_FRAME_PLANE_DATA(frame, 0),
                GST_VIDEO_FRAME_PLANE_STRIDE(frame, 0));

    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);

    std::vector<cv::Rect> faces;
    self->faceCascade.detectMultiScale(
        gray, faces,
        1.1, 5, 0 | cv::CASCADE_SCALE_IMAGE,
        cv::Size(30, 30), cv::Size(300, 300)
    );

    for (const auto &face : faces) {
        cv::Mat roi = img(face & cv::Rect(0, 0, img.cols, img.rows));
        cv::GaussianBlur(roi, roi, cv::Size(55, 55), 30);
    }

    return GST_FLOW_OK;
}

static void gst_finalplugin_class_init(GstFinalPluginClass *klass)
{
    GstElementClass *element_class = GST_ELEMENT_CLASS(klass);
    GstVideoFilterClass *video_filter_class = (GstVideoFilterClass *)klass;

    video_filter_class->transform_frame_ip = GST_DEBUG_FUNCPTR(gst_finalplugin_transform_frame_ip);

    gst_element_class_set_metadata(
        element_class,
        "Final Face Blur",                    // long-name
        "Filter/Effect/Video",                // klass
        "Blurs faces using Haar Cascade",     // description
        "Aiad Mesak <aiad@gmail.com>"       // author
    );

    gst_element_class_add_pad_template(element_class, gst_static_pad_template_get(&sink_template));
    gst_element_class_add_pad_template(element_class, gst_static_pad_template_get(&src_template));
}

static void gst_finalplugin_init(GstFinalPlugin *self)
{
    if (!self->faceCascade.load("/usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml")) {
        GST_ERROR("Failed to load Haar cascade file.");
    }
}

static gboolean plugin_init(GstPlugin *plugin)
{
    return gst_element_register(plugin, "finalplugin", GST_RANK_NONE, GST_TYPE_FINALPLUGIN);
}

GST_PLUGIN_DEFINE(
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    finalplugin,
    "Final Face Blur Plugin",
    plugin_init,
    "1.0",
    "LGPL",
    "finalplugin",
    "https://plugin.org"
)

