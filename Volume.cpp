#include "Volume.h"

Volume::Volume(std::vector<VoxelVertex>&& data, int width, int height, int depth) : 
    m_width(width), m_height(height), m_depth(depth) {
    m_data = data;
    calculate_gradient();
}

Volume::Volume(int width, int height, int depth) :
    m_width(width), m_height(height), m_depth(depth) {
    m_data = std::vector<VoxelVertex>(width * height * depth);
}

Volume::Volume(const Volume &other) {
    m_width = other.m_width;
    m_height = other.m_height;
    m_depth = other.m_depth;
    m_data = other.m_data;
}

Volume::~Volume() {}

VoxelVertex& Volume::get_voxel(int x, int y, int z) {
    return m_data[z * m_depth * m_height + (y * m_height + x)];
}

void Volume::calculate_gradient() {
    for (int z = 1; z < m_depth - 1; z++) {
        for (int y = 1; y < m_height - 1; y++) {
            for (int x = 1; x < m_width - 1; x++) {
                VoxelVertex& v = get_voxel(x, y, z);
                
                // X-axis gradient calculation
                VoxelVertex& x_before = get_voxel(x - 1, y, z);
                VoxelVertex& x_after = get_voxel(x + 1, y, z);
                v.gradient.x = (x_before.value - x_after.value) / 2.0f;

                // Y-axis gradient calculation
                VoxelVertex& y_before = get_voxel(x, y - 1, z);
                VoxelVertex& y_after = get_voxel(x, y + 1, z);
                v.gradient.y = (y_before.value - y_after.value) / 2.0f;

                // Z-axis gradient calculation
                VoxelVertex& z_before = get_voxel(x, y, z - 1);
                VoxelVertex& z_after = get_voxel(x, y, z + 1);
                v.gradient.z = (z_before.value - z_after.value) / 2.0f;

                v.gradient = glm::normalize(v.gradient);
            }
        }
    }
}

Volume create_perlin_volume(int size) {
    const siv::PerlinNoise perlin{ 123456u };
	auto noise = std::vector<VoxelVertex>(256 * 256 * 256);
	for (int yy = 0; yy < 16; yy++) {
		for (int xx = 0; xx < 16; xx++) {
			int index_1 = yy * 256 * 4096 + xx * 256;

			for (int y = 0; y < 256; y++) {
				for (int x = 0; x < 256; x++) {
					int index_2 = index_1 + (y * 4096 + x);
					noise[index_2] = VoxelVertex {
						(float)perlin.noise3D_01(x * 0.05f, y * 0.05f, (yy * 16 + xx) * 0.05f),
						glm::vec3(0, 0, 0),
					};
					//std::cout << i << ", " << j << ", " << k << " | " << noise[(i * 256 + j) * 256 + k] << std::endl;
				}
			}
		}
	}
	auto volume = Volume(std::move(noise), 256, 256, 256);
    return volume;
}
