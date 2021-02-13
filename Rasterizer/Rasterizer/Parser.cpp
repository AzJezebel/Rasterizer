#include "pch.h"
#include "Parser.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

#pragma warning(disable:26451)

Mesh* LoadOBJ(const std::string& path)
{
	Mesh* mesh = new Mesh();

	// Spaghetti code here
	std::string assetPath = "asset/OBJ/";

	std::ifstream ifi(assetPath + path);
	std::string mtl;
	std::string mtllib;

	std::vector<Vec3> temp_vtx;
	std::vector<Vec3> temp_nrm;
	std::vector<Vec3> temp_tex;

	std::vector<face> temp_faces;

	std::map<std::string, Material> temp_mat;

	if (!ifi)
	{
		std::cout << "\tError opening file '" << path << "'" << std::endl;
		return nullptr;
	}

	std::string line;

	std::cout << "\tLoading file: '" << path << "'" << std::endl;

	while (getline(ifi, line))
	{
		if (line.substr(0, 1) == "#")
		{
			continue;
		}
		else if (line.substr(0, 2) == "v ")
		{
			std::string s(line.substr(2));
			Vec3 v;
			sscanf_s(s.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
			temp_vtx.push_back(v);
		}
		else if (line.substr(0, 2) == "vn")
		{
			std::string s(line.substr(2));
			Vec3 n;
			sscanf_s(s.c_str(), "%f %f %f", &n.x, &n.y, &n.z);
			temp_nrm.push_back(n);
		}
		else if (line.substr(0, 2) == "vt")
		{
			std::string s(line.substr(2));
			Vec3 v;
			sscanf_s(s.c_str(), "%f %f", &v.x, &v.y);
			temp_tex.push_back(v);
		}
		else if (line.substr(0, 2) == "f ")
		{
			std::string s(line.substr(2));
			face f;

			if (mtl.empty())
			{
				sscanf_s(s.c_str(), "%d//%d %d//%d %d//%d",
					&f.vertex[0], &f.normal[0],
					&f.vertex[1], &f.normal[1],
					&f.vertex[2], &f.normal[2]);
			}
			else
			{
				sscanf_s(s.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d",
					&f.vertex[0], &f.texture[0], &f.normal[0],
					&f.vertex[1], &f.texture[1], &f.normal[1],
					&f.vertex[2], &f.texture[2], &f.normal[2]);
				f.mtl = mtl;
			}

			temp_faces.push_back(f);
		}
		else if (line.substr(0, 7) == "usemtl ")
		{
			mtl = line.substr(7);

			Material m = Material::ms_DEFAULT;

			temp_mat.insert(std::pair<std::string, Material>(mtl, m));
		}
		else if (line.substr(0, 7) == "mtllib ")
		{
			mtllib = line.substr(7);
		}
	}

	if (!mtllib.empty())
	{
		std::cout << "\tLoading mtl file: '" << mtllib << "'" << std::endl;

		std::ifstream ifi2(assetPath + mtllib);

		if (!ifi2)
		{
			std::cout << "\tError opening file '" << mtllib << "'" << std::endl;
		}

		std::string line2;

		while (getline(ifi2, line2))
		{
			if (line2.substr(0, 1) == "#")
			{
				continue;
			}
			else if (line2.substr(0, 7) == "newmtl ")
			{
				mtl = line2.substr(7);
			}
			else if (line2.substr(0, 3) == "Ka ")
			{
				std::string s(line2.substr(3));
				Vec3 v;
				sscanf_s(s.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
				temp_mat[mtl].ambient = v;
			}
			else if (line2.substr(0, 3) == "Kd ")
			{
				std::string s(line2.substr(3));
				Vec3 v;
				sscanf_s(s.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
				temp_mat[mtl].diffuse = v;
			}
			else if (line2.substr(0, 3) == "Ks ")
			{
				std::string s(line2.substr(3));
				Vec3 v;
				sscanf_s(s.c_str(), "%f %f %f", &v.x, &v.y, &v.z);
				temp_mat[mtl].specular = v;
			}
			else if (line2.substr(0, 3) == "Ns ")
			{
				std::string s(line2.substr(3));
				float sh;
				sscanf_s(s.c_str(), "%f", &sh);
				temp_mat[mtl].shininess = sh;
			}
			else if (line2.substr(0, 3) == "map")
			{
				std::string s(line2.substr(7));
				temp_mat[mtl].textureMapIndex = s;
				Texture* newtexture = new Texture((assetPath + s).c_str());
				mesh->pTextures.insert(std::pair<std::string, Texture*>(s, newtexture));
			}
		}
	}

	printf("\t\tAssembling %I64u faces\n", temp_faces.size());

	for (auto& f : temp_faces)
	{
		f.CheckValues((int) temp_vtx.size(), (int) temp_nrm.size(), (int) temp_tex.size());

		if (f.mtl.empty())
		{
			for (int i = 0; i < 3; i++)
			{
				mesh->AddVertexDuplicate({
					temp_vtx[(f.vertex[i]) - (size_t) 1],
					temp_nrm[(f.normal[i]) - 1],
					});
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				mesh->AddVertexDuplicate({
					temp_vtx[(f.vertex[i]) - 1],
					temp_nrm[(f.normal[i]) - 1],
					temp_tex[(f.texture[i]) - 1].x,
					temp_tex[(f.texture[i]) - 1].y,
					Color::ms_WHITE,
					temp_mat[f.mtl]
					});
			}
		}
	}
	printf("\t\tAssembled %I64u faces\n", temp_faces.size());

	return mesh;
}

void face::CheckValues(int v, int n, int t)
{
	for (int i = 0; i < 3; i++)
	{
		vertex[i] = MakePositive(vertex[i], v);
	}

	for (int i = 0; i < 3; i++)
	{
		texture[i] = MakePositive(texture[i], t);
	}

	for (int i = 0; i < 3; i++)
	{
		normal[i] = MakePositive(normal[i], n);
	}
}

int face::MakePositive(int num, int size)
{
	if (num >= 0)
		return num;

	return (size + num + 1);
}