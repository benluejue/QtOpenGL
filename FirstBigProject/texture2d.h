#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <QOpenGLTexture>
#include "utils.h"

enum TextureType { DIFFUSE, SPECULAR };
class Texture2D
{
	friend class ResourceManager;
public:
	Texture2D();
	Texture2D(TextureType type, std::string path);
	~Texture2D();
	void generate(const QString& file);
	void bind() const;
	std::string path() const;
	TextureType type() const;

	QOpenGLTexture::TextureFormat internal_format;//Format of texture object
	QOpenGLTexture::WrapMode wrap_s;
	QOpenGLTexture::WrapMode wrap_t;
	QOpenGLTexture::Filter filter_min;
	QOpenGLTexture::Filter filter_max;
private:
	QOpenGLTexture* texture;
	TextureType _type;
	std::string _path;
};

#endif // TEXTURE2D_H
