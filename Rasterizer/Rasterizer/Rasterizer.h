#pragma once

#include "Scene.h"
#include "Texture.h"
#include "Entity.h"
#include "Point2d.h"
#include "Camera.h"
#include "Material.h"

class Rasterizer
{
public:

	void RenderScene(Scene* pScene, Texture* pTarget);

	void ToggleWireframe();
	void ToggleBackfaceCulling();
	void ToggleXRot();
	void ToggleYRot();
	void ToggleZRot();

	bool changeSize = true;

	bool xRot = false;
	bool yRot = false;
	bool zRot = false;

	Camera& GetCamera() const;
	static Vec3 rotation;

private:
	static Camera camera;
	static Point2d BoxMin;
	static Point2d BoxMax;
	float* zbuffer = nullptr;

	size_t height = 0;
	size_t width = 0;

	std::vector<Vertex> vertices;
	std::vector<int> indices;
	bool ccw = false;

	float GetZValue(const float w0, const float w1, const float w2, const float z0, const float z1, const float z2) const;

	void DrawTriLines(const Vertex& v0, const Vertex& v1, const Vertex& v2, Texture* pTarget, Scene* pScene, std::pair<Vec3, Vec3>& boundingBox, const std::map<std::string, Texture*>& textures) const;
	void DrawLine(const Vertex& v0, const Vertex& v1, Texture* pTarget, Scene* pScene, std::pair<Vec3, Vec3>& boundingBox, const std::map<std::string, Texture*>& textures) const;

	void DrawTri(const Vertex& v0, const Vertex& v1, const Vertex& v2, Texture* pTarget, Scene* pScene, const std::map<std::string, Texture*>& textures, float entityAlpha, std::pair<Vec3, Vec3>& boundingBox) const;

	const Color GetLightingColor(const Vec3& normal, const std::vector<Light> lights, const Vec3& pixelPos, const Color& pColor, const Point2d& screensize, const Material& mat) const;

	Vec3 GetNormalInterpolation(const float w0, const float w1, const float w2, const Vec3& n0, const Vec3& n1, const Vec3& n2) const;

	static float mInfinity;
	//static Mat4 projectionMat;

	bool showWireframe = false;
	bool backfaceCulling = true;

	std::pair<Vec3, Vec3> CreateBoundingBox(const Vertex& v0, const Vertex& v1, const Vertex& v2, Point2d screenSize, Texture* pTarget, Scene* pScene) const;
	bool ClipPolygonAxis(std::vector<Vec3>& vertices1, std::vector<Vec3>& vertices2, int component) const;
	void ClipPolygonComponent(std::vector<Vec3>& vertices, int component, float componentFactor, std::vector<Vec3>& result) const;
};

void SizeToScreen(std::pair<Vec3, Vec3>& boundingBox, const Point2d& windowSize);

void ProjectViewPort(Vertex& v, const Point2d& windowSize);
void ProjectViewPort(Vec3& v, const Point2d& windowSize);

void GetBary(float& w0, float& w1, float& w2, Point2d& p, Point2d& p0, Point2d& p1, Point2d& p2);

int Min3(int i1, int i2, int i3);
int Max3(int i1, int i2, int i3);
