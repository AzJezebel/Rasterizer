#include "pch.h"
#include "Rasterizer.h"
#include <list>

#include <algorithm>
#include <array>
#include "Material.h"

#pragma warning(disable:26451)

float Rasterizer::mInfinity = std::numeric_limits<float>::infinity();
Vec3 Rasterizer::rotation = { 0, 0, 0 };
Camera Rasterizer::camera;

//Displays our scene on the screen arg = (Scene*, Texture*)
void Rasterizer::RenderScene(Scene* pScene, Texture* pTarget)
{
	if (pScene == nullptr)
		return;

	const Point2d windowSize(pTarget->GetSize());
	bool clipped = false;
	std::vector<Vertex> newList;
	std::array<Vertex, 3> triangle;
	Vertex newWindowSize = { (float) windowSize.x, (float) windowSize.y, 0.f };

	if (changeSize)
	{
		changeSize = false;
		width = windowSize.x;
		height = windowSize.y;

		delete zbuffer;

		zbuffer = new float[width * height];
	}

	Mat4 projectionMat = Mat4::CreateProjectionMatrix(0.1f, 10.f, (width / height));

	int arraySize = (int) width * (int) height;

	for (int i = 0; i < arraySize; i++)
	{
		zbuffer[i] = mInfinity;
	}

	for (auto& entity : pScene->entities)
	{
		if (!entity.GetMesh())
			continue;

		Vec3 rot = entity.GetRotation();

		if (xRot)
			rot.x += 1;
		if (yRot)
			rot.y += 1;
		if (zRot)
			rot.z += 1;

		entity.SetRotation(rot);
		Mat4 transform = Mat4::CreateTransformMatrix(entity.GetTranslation(), entity.GetRotation(), entity.GetScale());
		Mat4 cameraMat = Mat4::CreateCameraMatrix(camera.GetPosition(), camera.GetRotation());
		Mat4 transformRot = Mat4::CreateTransformMatrix({ 0, 0, 0 }, entity.GetRotation(), { 1, 1, 1 });

		int numTriangles = entity.GetMesh()->NumberOfTriangles();

		for (int i = 0; i < numTriangles; i++)
		{
			triangle = entity.GetMesh()->GetTriangle(i);

			for (int j = 0; j < 3; j++)
			{
				triangle[j].position = projectionMat * cameraMat * transform * triangle[j].position;
				triangle[j].normal = transformRot * triangle[j].normal;
			}

			std::pair<Vec3, Vec3> boundingBox = CreateBoundingBox(triangle[0], triangle[1], triangle[2], windowSize, pTarget, pScene);
			SizeToScreen(boundingBox, windowSize);

			for (int j = 0; j < 3; j++)
			{
				ProjectViewPort(triangle[j], windowSize);
			}

			bool draw = true;
			if (backfaceCulling)
			{
				Vec3 faceNormal = Cross(
					(triangle[1].position - triangle[0].position),
					(triangle[2].position - triangle[0].position)
				);

				draw = faceNormal.z <= 0;
			}

			if (draw)
			{
				if (showWireframe)
					DrawTriLines(triangle[0], triangle[1], triangle[2], pTarget, pScene, boundingBox, entity.GetMesh()->pTextures);
				else
					DrawTri(triangle[0], triangle[1], triangle[2], pTarget, pScene, entity.GetMesh()->pTextures, entity.GetAlpha(), boundingBox);
			}
		}
	}
}

//Enable/Disable Wireframe on the entities
void Rasterizer::ToggleWireframe()
{
	showWireframe = !showWireframe;
}

//Enable/Disable BackfaceCulling on the entities
void Rasterizer::ToggleBackfaceCulling()
{
	backfaceCulling = !backfaceCulling;
}

void Rasterizer::ToggleXRot()
{
	xRot = !xRot;
}

void Rasterizer::ToggleYRot()
{
	yRot = !yRot;
}

void Rasterizer::ToggleZRot()
{
	zRot = !zRot;
}

float Rasterizer::GetZValue(const float w0, const float w1, const float w2, const float z0, const float z1, const float z2) const
{
	return z0 * w0 + z1 * w1 + z2 * w2;
}

//Used to draw the lines of our entitie's triangles 
void Rasterizer::DrawTriLines(const Vertex& v0, const Vertex& v1, const Vertex& v2, Texture* pTarget, Scene* pScene, std::pair<Vec3, Vec3>& boundingBox, const std::map<std::string, Texture*>& textures) const
{
	DrawLine(v0, v1, pTarget, pScene, boundingBox, textures);
	DrawLine(v0, v2, pTarget, pScene, boundingBox, textures);
	DrawLine(v2, v1, pTarget, pScene, boundingBox, textures);
}

//Use to display entities in wireframe (Works with textures and lights)
void Rasterizer::DrawLine(const Vertex& v0, const Vertex& v1, Texture* pTarget, Scene* pScene, std::pair<Vec3, Vec3>& boundingBox, const std::map<std::string, Texture*>& textures) const
{
	Point2d screenSize(pTarget->GetSize());

	bool changed = false;

	Point2d p(v0);

	int deltax = (int) abs(v1.position.x - v0.position.x);
	int deltay = (int) abs(v1.position.y - v0.position.y);

	int signx = (v1.position.x > v0.position.x ? 1 : -1);
	int signy = (v1.position.y > v0.position.y ? 1 : -1);

	bool interpolateZ = (v0.position.z == v1.position.z);
	bool interpolateColor = (v0.color == v1.color);
	bool interpolateNormal = (v0.normal == v1.normal);

	if (deltay > deltax)
	{
		std::swap(deltax, deltay);
		changed = true;
	}

	int e = 2 * deltay - deltax;

	for (int i = 1; i <= deltax; i++)
	{
		if (p.x >= 0 && p.y >= 0 && p.x < screenSize.x && p.y < screenSize.y)
		{
			float num = sqrtf((p.x - v0.position.x) * (p.x - v0.position.x) + (p.y - v0.position.y) * (p.y - v0.position.y));
			float denom = sqrtf((v1.position.x - v0.position.x) * (v1.position.x - v0.position.x) + (v1.position.y - v0.position.y) * (v1.position.y - v0.position.y));

			float t1 = num / denom;
			float t0 = 1.f - t1;

			float z = t0 * v0.position.z + t1 * v1.position.z;

			int offset = ((int) width) * p.y + p.x;

			if (z < zbuffer[offset])
			{
				zbuffer[offset] = z;

				Color c;

				if (!v0.mat.textureMapIndex.empty())
				{
					const std::string texIndex = v0.mat.textureMapIndex;
					const Color* newTexture = textures.at(texIndex)->GetPixels();

					unsigned int width = textures.at(texIndex)->GetSize().x;
					unsigned int height = textures.at(texIndex)->GetSize().y;

					float u = (float) width * (v0.uPos * t1 + v1.uPos * t0);
					float v = (float) height * (v0.vPos * t1 + v1.vPos * t0);

					int uC00 = (int) (u - 0.5f);
					int vC00 = (int) (v - 0.5f);

					float uFrac = u - (float) uC00 - 0.5f;
					float vFrac = v - (float) vC00 - 0.5f;

					Color C00 = newTexture[uC00 + vC00 * width];

					int uC10 = uC00 + 1;
					Color C10 = newTexture[uC10 + vC00 * width];

					int vC01 = vC00 + 1;
					Color C01 = newTexture[uC00 + vC01 * width];

					int uC11 = uC10;
					int vC11 = vC01;
					Color C11 = newTexture[uC11 + vC11 * width];

					c = (C00 * (1 - uFrac)) * (1 - vFrac) + (C10 * uFrac) * (1 - vFrac) + (C01 * (1 - uFrac))
						* vFrac + (C11 * uFrac) * vFrac;
				}

				else if (!interpolateColor)
					Color c = GetInterpolationColorLine(t0, t1, v0.color, v1.color);
				else
					c = v0.color;

				Vec3 n;
				if (!interpolateNormal)
					n = Normalize(v0.normal * t0 + v1.normal * t1);
				else
					n = v0.normal;

				c = GetLightingColor(n, pScene->lights, { (float) p.x, (float) p.y, z }, c, screenSize, v0.mat);

				pTarget->SetPixelColor(p.x, p.y, c);
			}
		}

		if (e >= 0)
		{
			if (changed)
				p.x += signx;
			else
				p.y += signy;
			e -= 2 * deltax;
		}
		if (changed)
			p.y += signy;
		else
			p.x += signx;
		e += 2 * deltay;
	}
}

// Pass pre-projected points (not in world space but screen space)
void Rasterizer::DrawTri(const Vertex& v0, const Vertex& v1, const Vertex& v2, Texture* pTarget, Scene* pScene, const std::map<std::string, Texture*>& textures, float entityAlpha, std::pair<Vec3, Vec3>& boundingBox) const
{
	Point2d p0(v0);
	Point2d p1(v1);
	Point2d p2(v2);

	bool interpolateZ = (v0.position.z == v1.position.z && v1.position.z == v2.position.z);
	bool interpolateColor = (v0.color == v1.color && v1.color == v2.color);
	bool interpolateNormal = (v0.normal == v1.normal && v1.normal == v2.normal);

	int minX = boundingBox.first.x;
	int minY = boundingBox.second.y;
	int maxX = boundingBox.second.x;
	int maxY = boundingBox.first.y;

	Point2d screenSize = pTarget->GetSize();

	float w0, w1, w2;
	Point2d p(0, 0);

	// Check axes, if point is inside or on all 3 axes, draw
	for (p.y = minY; p.y <= maxY; p.y++)
	{
		for (p.x = minX; p.x <= maxX; p.x++)
		{
			GetBary(w0, w1, w2, p, p0, p1, p2);

			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
			{
				float z;
				if (!interpolateZ)
					z = GetZValue(w0, w1, w2, v0.position.z, v1.position.z, v2.position.z);
				else
					z = v0.position.z;

				int offset = ((int) width) * p.y + p.x;

				if (z < zbuffer[offset])
				{
					zbuffer[offset] = z;

					Color c;

					if (!v0.mat.textureMapIndex.empty())
					{
						const std::string texIndex = v0.mat.textureMapIndex;
						const Color* newTexture = textures.at(texIndex)->GetPixels();

						unsigned int width = textures.at(texIndex)->GetSize().x;
						unsigned int height = textures.at(texIndex)->GetSize().y;

						float u = (float) width * (v0.uPos * w0 + v1.uPos * w1 + v2.uPos * w2);
						float v = (float) height * (v0.vPos * w0 + v1.vPos * w1 + v2.vPos * w2);

						int uC00 = (int) (u - 0.5f);
						int vC00 = (int) (v - 0.5f);

						float uFrac = u - (float) uC00 - 0.5f;
						float vFrac = v - (float) vC00 - 0.5f;

						Color C00 = newTexture[uC00 + vC00 * width];

						int uC10 = uC00 + 1;
						Color C10 = newTexture[uC10 + vC00 * width];

						int vC01 = vC00 + 1;
						Color C01 = newTexture[uC00 + vC01 * width];

						int uC11 = uC10;
						int vC11 = vC01;
						Color C11 = newTexture[uC11 + vC11 * width];

						c = (C00 * (1 - uFrac)) * (1 - vFrac) + (C10 * uFrac) * (1 - vFrac) + (C01 * (1 - uFrac))
							* vFrac + (C11 * uFrac) * vFrac;
					}
					else if (!interpolateColor)
					{
						c = GetInterpolationColor(w0, w1, w2, v0.color, v1.color, v2.color, entityAlpha);
					}
					else
						c = v0.color;

					Vec3 n;
					if (!interpolateNormal)
						n = GetNormalInterpolation(w0, w1, w2, v0.normal, v1.normal, v2.normal);
					else
						n = v0.normal;

					c = GetLightingColor(n, pScene->lights, { (float) p.x, (float) p.y, z }, c, screenSize, v0.mat);

					pTarget->SetPixelColor(p.x, p.y, c);
				}
			}
		}
	}
}

//Get barycentric points based of the original vertices
void GetBary(float& w0, float& w1, float& w2, Point2d& p, Point2d& p0, Point2d& p1, Point2d& p2)
{
	float denom = (float) (p1.y - p2.y) * (p0.x - p2.x) + (p2.x - p1.x) * (p0.y - p2.y);

	w0 = ((p1.y - p2.y) * (p.x - p2.x) + (p2.x - p1.x) * (p.y - p2.y)) / denom;
	w1 = ((p2.y - p0.y) * (p.x - p2.x) + (p0.x - p2.x) * (p.y - p2.y)) / denom;
	w2 = ((p0.y - p1.y) * (p.x - p0.x) + (p1.x - p0.x) * (p.y - p0.y)) / denom;
}

//Transforms vertex from clip space to view space
void ProjectViewPort(Vertex& v, const Point2d& windowSize)
{
	v.position.x = ((v.position.x) + 1.f) * 0.5f * ((float) windowSize.x);
	v.position.y = (windowSize.y - ((v.position.y) + 1.f) * 0.5f * ((float) windowSize.y));
}

//Transforms 3Dvector from clip space to view space
void ProjectViewPort(Vec3& v, const Point2d& windowSize)
{
	v.x = ((v.x) + 1.f) * 0.5f * ((float) windowSize.x);
	v.y = (windowSize.y - ((v.y) + 1.f) * 0.5f * ((float) windowSize.y));
}

//Returns the minimum between 3 integers
int Min3(int i1, int i2, int i3)
{
	return std::min(std::min(i1, i2), i3);
}

//Returns the maximum between 3 integers
int Max3(int i1, int i2, int i3)
{
	return std::max(std::max(i1, i2), i3);
}

//Return thre color of the light color on a point of the entity
const Color Rasterizer::GetLightingColor(const Vec3& normal, const std::vector<Light> lights, const Vec3& pixelPos, const Color& pColor, const Point2d& screensize, const Material& mat) const
{
	// values are rgb intensity totals
	Vec3 intensity = { 0.f, 0.f, 0.f };

	for (const auto& light : lights)
	{
		if (light.onoff)
		{
			// TODO: Fix this shit
			Vertex lightpos(light.position);
			ProjectViewPort(lightpos, screensize);
			Vec3 lightToPoint = Normalize(lightpos.position - pixelPos);

			Vec3 cameraToPoint = Normalize(GetCamera().GetPosition() - pixelPos);
			Vec3 reflection = Normalize((normal * (Dot(lightToPoint, normal) * 2.f) - lightToPoint));

			// TODO: replace 1
			for (int i = 0; i < 3; i++)
			{
				float ambient = light.ambientComponent[i] * mat.ambient[i];
				float diffuse = light.diffuseComponent[i] * mat.diffuse[i] * (Dot(lightToPoint, normal));
				float specular = light.specularComponent[i] * mat.specular[i] * powf(std::max(0.f, Dot(cameraToPoint, reflection)), mat.shininess);

				intensity[i] += ambient + diffuse + specular;
			}
		}
	}

	if (intensity.x == 0.f && intensity.y == 0.f && intensity.z == 0.f)
		return pColor;
	else
		return pColor * intensity;
}

//Interpolates normals
Vec3 Rasterizer::GetNormalInterpolation(const float w0, const float w1, const float w2, const Vec3& n0, const Vec3& n1, const Vec3& n2) const
{
	return Normalize((n0 * w0) + (n1 * w1) + (n2 * w2));
}

Camera& Rasterizer::GetCamera() const
{
	return camera;
}

//Used in clipping algo, creates the bounding box of the current entity
std::pair<Vec3, Vec3> Rasterizer::CreateBoundingBox(const Vertex& v0, const Vertex& v1, const Vertex& v2, Point2d screenSize, Texture* pTarget, Scene* pScene) const
{
	std::vector<Vec3> vertices1;
	vertices1.push_back(v0.position);
	vertices1.push_back(v1.position);
	vertices1.push_back(v2.position);

	std::vector<Vec3> vertices2;

	float minX;
	float minY;
	float maxX;
	float maxY;

	if (ClipPolygonAxis(vertices1, vertices2, 0) &&
		ClipPolygonAxis(vertices1, vertices2, 1) &&
		ClipPolygonAxis(vertices1, vertices2, 2))
	{
		minX = vertices1[0].x;
		minY = vertices1[0].y;
		maxX = vertices1[0].x;
		maxY = vertices1[0].y;

		for (auto& v : vertices1)
		{
			minX = std::min(minX, v.x);
			minY = std::min(minY, v.y);
			maxX = std::max(maxX, v.x);
			maxY = std::max(maxY, v.y);
		}

		Vec3 nv0({ minX, minY, 0.f });
		Vec3 nv1({ minX, maxY, 0.f });
		Vec3 nv2({ maxX, maxY, 0.f });
		Vec3 nv3({ maxX, minY, 0.f });

		ProjectViewPort(nv0, screenSize);
		ProjectViewPort(nv1, screenSize);
		ProjectViewPort(nv2, screenSize);
		ProjectViewPort(nv3, screenSize);

		return std::pair<Vec3, Vec3>(nv0, nv2);
	}
	else
	{
		Vec3 min((float) Min3(v0.position.x, v1.position.x, v2.position.x), (float) Min3(v0.position.y, v1.position.y, v2.position.y), 0.f);
		Vec3 max((float) Max3(v0.position.x, v1.position.x, v2.position.x), (float) Max3(v0.position.y, v1.position.y, v2.position.y), 0.f);
		return std::pair<Vec3, Vec3>(min, max);
	}
}

//Called to clip entities on edges of the screenspace return false if nothing is clipped
bool Rasterizer::ClipPolygonAxis(std::vector<Vec3>& vertices1, std::vector<Vec3>& vertices2, int component) const
{
	ClipPolygonComponent(vertices1, component, 1.f, vertices2);
	vertices1.clear();

	if (vertices2.size() == 0)
	{
		return false;
	}

	ClipPolygonComponent(vertices2, component, -1.f, vertices1);
	vertices2.clear();

	return !(vertices1.size() == 0);
}

//Test if all vertices are inside or outside the clipping space else clips the entity
void Rasterizer::ClipPolygonComponent(std::vector<Vec3>& vertices, int component, float componentFactor, std::vector<Vec3>& result) const
{
	Vec3 previousVertex = vertices[vertices.size() - 1];
	float previousComponent = previousVertex[component] * componentFactor;

	bool previousInside = previousComponent <= 1.f;

	for (auto& currentVertex : vertices)
	{
		float currentComponent = currentVertex[component] * componentFactor;
		bool currentInside = currentComponent <= 1.f;

		if (currentInside ^ previousInside)
		{
			float lerpAmt = (1 - previousComponent) /
				((1 - previousComponent) - (1 - currentComponent));

			previousVertex.Lerp(currentVertex, lerpAmt);
			result.push_back(previousVertex);
		}
		if (currentInside)
		{
			result.push_back(currentVertex);
		}

		previousVertex = currentVertex;
		previousComponent = currentComponent;
		previousInside = currentInside;
	}
}

//Called to define clipping edges and set threshold for the vertices to display
void SizeToScreen(std::pair<Vec3, Vec3>& boundingBox, const Point2d& windowSize)
{
	boundingBox.first.x = std::min(std::max(boundingBox.first.x, 0.f), (float) windowSize.x - 1);
	boundingBox.first.y = std::min(std::max(boundingBox.first.y, 0.f), (float) windowSize.y - 1);
	boundingBox.second.x = std::max(std::min(boundingBox.second.x, (float) windowSize.x - 1), 0.f);
	boundingBox.second.y = std::max(std::min(boundingBox.second.y, (float) windowSize.y - 1), 0.f);
}