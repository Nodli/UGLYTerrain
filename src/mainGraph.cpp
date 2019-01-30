#include <iostream>
#include <MultiLayerMap.hpp>
#include <Noise/TerrainNoise.hpp>
#include <Weather/Erosion.hpp>
#include <Weather/Biome.hpp>
#include <Weather/Hydro.hpp>
#include <Vegetation/Vegetation.hpp>

#include <ImGui/imgui.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ImGui/imgui_demo.cpp>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

struct Parameters
{
	TerrainNoise t_noise = TerrainNoise(5.0, 1.0 / 200.0, 8);
	char saveName[256] = "default";
	int seed1 = 0;
	int seed2 = 4;
	int sizeWidth = 500;
	int sizeHeight = 500;
	Eigen::Vector2d posMin;//(-25, -25);
	Eigen::Vector2d posMax;//(25, 25);
};

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* set_up_window(const int width = 600, const int height = 400, std::string title = "pata patate")
{
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);

	if(!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	if(window == NULL)
	{
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
	bool err = glewInit() != GLEW_OK;

	if(err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		exit(EXIT_FAILURE);
	}

	return window;
}

void set_up_imgui(GLFWwindow* window)
{
	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	// Setup style
	ImGui::StyleColorsDark();
}

void export_tab(const SimpleLayerMap& sf, const std::string& name)
{
	if(ImGui::CollapsingHeader("Exports"))
	{
		if(ImGui::Button("Export as obj"))
		{
			sf.export_as_obj(name + ".obj");
		}

		if(ImGui::Button("Export as pgm"))
		{
			sf.export_as_pgm(name + ".pgm");
		}

		if(ImGui::Button("Export slope as pgm"))
		{
			SimpleLayerMap::generate_slope_map(sf).export_as_pgm(name + "_slope_" + ".pgm");
		}

		if(ImGui::Button("Export area as pgm"))
		{
			get_area(sf).export_as_pgm(name + "_area_" + ".pgm");
		}

		if(ImGui::Button("Export hydro as pgm"))
		{
			get_water_indexes(sf).export_as_pgm(name + "_hydro_" + ".pgm");
		}

		if(ImGui::Button("Export expo as pgm"))
		{
			get_light_exposure(sf).export_as_pgm(name + "_expo_" + ".pgm");
		}
	}
}

void multi_layer_map_window(MultiLayerMap& mlm, Parameters& params)
{
	ImGui::Begin("Terrain");                         // Create a window called "Hello, world!" and append into it.
	ImGui::PushItemWidth(100);
	ImGui::InputText("export name", params.saveName, IM_ARRAYSIZE(params.saveName));

	if(ImGui::CollapsingHeader("Presets"))
	{
		if(ImGui::Button("Big Test"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
		{
			strcpy(params.saveName, "big");
			params.sizeWidth = 500;
			params.sizeHeight = 500;
			params.posMin = Eigen::Vector2d(-25, -25);
			params.posMax = Eigen::Vector2d(25, 25);
			params.t_noise._amplitude = 10.0;
			params.t_noise._base_freq = 0.005;
			params.t_noise._octaves = 8;
			params.seed1 = 0;
			params.seed2 = 4;
		}

		if(ImGui::Button("Huge Test"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
		{
			strcpy(params.saveName, "big");
			params.sizeWidth = 1000;
			params.sizeHeight = 1000;
			params.posMin = Eigen::Vector2d(-50, -50);
			params.posMax = Eigen::Vector2d(50, 50);
			params.t_noise._amplitude = 10.0;
			params.t_noise._base_freq = 0.005;
			params.t_noise._octaves = 8;
			params.seed1 = 0;
			params.seed2 = 4;
		}

		if(ImGui::Button("Quick Test"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
		{
			strcpy(params.saveName, "quick");
			params.sizeWidth = 100;
			params.sizeHeight = 100;
			params.posMin = Eigen::Vector2d(-5, -5);
			params.posMax = Eigen::Vector2d(5, 5);
			params.t_noise._amplitude = 2.5;
			params.t_noise._base_freq = 1.0 / 100.0;
			params.t_noise._octaves = 8;
			params.seed1 = 0;
			params.seed2 = 4;
		}

		if(ImGui::Button("Quick Test with veget"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
		{
			strcpy(params.saveName, "quick");
			params.sizeWidth = 100;
			params.sizeHeight = 100;
			params.posMin = Eigen::Vector2d(-5, -5);
			params.posMax = Eigen::Vector2d(5, 5);
			params.t_noise._amplitude = 2.5;
			params.t_noise._base_freq = 1.0 / 100.0;
			params.t_noise._octaves = 8;
			params.seed1 = 0;
			params.seed2 = 4;
			std::cout << "generation" << std::endl;
			//SimpleLayerMap sf(sizeWidth, sizeHeight, posMin, posMax);
			mlm = MultiLayerMap(params.sizeWidth, params.sizeHeight, params.posMin, params.posMax);
			mlm.new_layer();
			params.t_noise._base_noise.SetSeed(params.seed1);
			params.t_noise._ridge_noise.SetSeed(params.seed2);

			for(int j = 0; j < params.sizeHeight; ++j)
			{
				for(int i = 0; i < params.sizeWidth; i++)
				{
					mlm.get_field(0).at(i, j) = params.t_noise.get_noise(i, j);
				}
			}

			// std::cout << "erotion" << std::endl;
			erode_using_exposure(mlm, 0.1);
			transport(mlm, 20);
			std::cout << "vegetation" << std::endl;
			simulate(mlm);
		}
	}

	if(ImGui::CollapsingHeader("Caracteristics"))
	{
		ImGui::InputInt("Width", &params.sizeWidth);
		ImGui::InputInt("Height", &params.sizeHeight);
		ImGui::InputInt("Seed 1", &params.seed1);
		ImGui::InputInt("Seed 2", &params.seed2);
		ImGui::InputDouble("minX", &params.posMin(0));
		ImGui::SameLine();
		ImGui::InputDouble("minY", &params.posMin(1));
		ImGui::InputDouble("maxX", &params.posMax(0));
		ImGui::SameLine();
		ImGui::InputDouble("maxY", &params.posMax(1));
		ImGui::InputDouble("Amplitude max", &params.t_noise._amplitude);
		ImGui::InputDouble("Base frequency", &params.t_noise._base_freq);
		ImGui::InputInt("Nb iterations", &params.t_noise._octaves);

		if(ImGui::Button("Generate 1"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
		{
			//SimpleLayerMap sf(sizeWidth, sizeHeight, posMin, posMax);
			mlm = MultiLayerMap(params.sizeWidth, params.sizeHeight, params.posMin, params.posMax);
			mlm.new_layer();
			params.t_noise._base_noise.SetSeed(params.seed1);
			params.t_noise._ridge_noise.SetSeed(params.seed2);

			for(int j = 0; j < params.sizeHeight; ++j)
			{
				for(int i = 0; i < params.sizeWidth; i++)
				{
					mlm.get_field(0).at(i, j) = params.t_noise.get_noise(i, j);
				}
			}
		}

		if(ImGui::Button("Generate 2"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
		{
			//SimpleLayerMap sf(sizeWidth, sizeHeight, posMin, posMax);
			mlm = MultiLayerMap(params.sizeWidth, params.sizeHeight, params.posMin, params.posMax);
			mlm.new_layer();
			params.t_noise._base_noise.SetSeed(params.seed1);
			params.t_noise._ridge_noise.SetSeed(params.seed2);

			for(int j = 0; j < params.sizeHeight; ++j)
			{
				for(int i = 0; i < params.sizeWidth; i++)
				{
					mlm.get_field(0).at(i, j) = params.t_noise.get_noise2(i, j);
				}
			}
		}

		if(ImGui::Button("Generate 3"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
		{
			//SimpleLayerMap sf(sizeWidth, sizeHeight, posMin, posMax);
			mlm = MultiLayerMap(params.sizeWidth, params.sizeHeight, params.posMin, params.posMax);
			mlm.new_layer();
			params.t_noise._base_noise.SetSeed(params.seed1);
			params.t_noise._ridge_noise.SetSeed(params.seed2);

			for(int j = 0; j < params.sizeHeight; ++j)
			{
				for(int i = 0; i < params.sizeWidth; i++)
				{
					mlm.get_field(0).at(i, j) = params.t_noise.get_noise3(i, j);
				}
			}
		}
	}

	if(ImGui::CollapsingHeader("Operations"))
	{
		if(ImGui::TreeNode("Erosion"))
		{
			static double erosion_factor = 0.1;
			static int iterations = 1;
			static double rest_angle = 45;
			ImGui::InputDouble("Erosion Factor", &erosion_factor);
			ImGui::InputInt("Iterations", &iterations);
			ImGui::InputDouble("Rest angle", &rest_angle);

			if(ImGui::Button("Erode median slope"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
			{
				erode_using_median_slope(mlm, erosion_factor);
			}

			if(ImGui::Button("Erode exposure"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
			{
				erode_using_exposure(mlm, erosion_factor);
			}

			if(ImGui::Button("transport"))
			{
				transport(mlm, rest_angle);
			}

			ImGui::TreePop();
		}

		if(ImGui::TreeNode("Vegetation"))
		{
			if(ImGui::Button("Export density as ppm"))
			{
				generate_distribution(mlm);
			}

			if(ImGui::Button("Simulate"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
			{
				simulate(mlm);
			}

			ImGui::TreePop();
		}
	}

	export_tab(mlm.generate_field(), std::string(params.saveName));

	if(ImGui::Button("Save"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		std::string filename = std::string(params.saveName) + ".mlm";
		std::ofstream output(filename, std::ofstream::out);
		output << mlm;
	}

	if(ImGui::Button("Import"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		std::string filename = std::string(params.saveName) + ".mlm";
		std::ifstream input(filename, std::ifstream::in);
		input >> mlm;
	}

	ImGui::End();
}

int main()
{
	Parameters params;
	MultiLayerMap mlm(500, 500);
	GLFWwindow* window = set_up_window();
	set_up_imgui(window);
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		multi_layer_map_window(mlm, params);
		ImGui::Begin("Layers");

		for(int l = 0; l < mlm.get_layer_number(); ++l)
		{
			if(ImGui::CollapsingHeader((std::string("Layer_") + std::to_string(l)).c_str()))// std::string("Layer _ " + l).c_str()))
			{
				export_tab(mlm.get_field(l), std::string(params.saveName) +
						   std::string("_layer_") + std::to_string(l));
			}
		}

		ImGui::End();
		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

