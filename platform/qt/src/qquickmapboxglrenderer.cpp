#include "qquickmapboxglrenderer.hpp"

#include <QMapboxGL>

#include <QSize>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QQuickWindow>

QQuickMapboxGLRenderer::QQuickMapboxGLRenderer()
{
    QMapboxGLSettings settings;
    settings.setAccessToken(qgetenv("MAPBOX_ACCESS_TOKEN"));
    settings.setCacheDatabasePath("/tmp/mbgl-cache.db");
    settings.setCacheDatabaseMaximumSize(20 * 1024 * 1024);

    m_map.reset(new QMapboxGL(nullptr, settings));
}

QQuickMapboxGLRenderer::~QQuickMapboxGLRenderer()
{
}

QOpenGLFramebufferObject* QQuickMapboxGLRenderer::createFramebufferObject(const QSize &size)
{
    m_map->resize(size);

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

    return new QOpenGLFramebufferObject(size, format);
}

void QQuickMapboxGLRenderer::render()
{
    m_map->render();
}

void QQuickMapboxGLRenderer::synchronize(QQuickFramebufferObject *item)
{
    if (!m_initialized) {
        m_map->setStyleURL(QMapbox::defaultStyles()[0].first);
        QObject::connect(m_map.data(), SIGNAL(needsRendering()), item, SLOT(update()));
        m_initialized = true;
    }
}
