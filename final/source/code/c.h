#ifndef GST_FINALPLUGIN_H
#define GST_FINALPLUGIN_H

#include <gst/video/gstvideofilter.h>
#include <opencv2/objdetect.hpp>

G_BEGIN_DECLS

#define GST_TYPE_FINALPLUGIN (gst_finalplugin_get_type())
G_DECLARE_FINAL_TYPE(GstFinalPlugin, gst_finalplugin, GST, FINALPLUGIN, GstVideoFilter)

struct _GstFinalPlugin
{
    GstVideoFilter parent;
    cv::CascadeClassifier faceCascade;
};

struct _GstFinalPluginClass
{
    GstVideoFilterClass parent_class;
};

G_END_DECLS

#endif // GST_FINALPLUGIN_H

