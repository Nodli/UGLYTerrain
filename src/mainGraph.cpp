#include <iostream>
#include <MultiLayerMap.hpp>
#include <Noise/TerrainNoise.hpp>

#include <ImGui/imgui.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ImGui/imgui_demo.cpp>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

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

int main()
{
	char saveName[256] = "default";
	int sizeWidth = 500;
	int sizeHeight = 500;
	Eigen::Vector2d posMin(-25, -25);
	Eigen::Vector2d posMax(25, 25);
	TerrainNoise t_noise(5.0, 1.0 / 200.0, 8);
	MultiLayerMap mlm(500, 500, posMin, posMax);
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
		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;
			ImGui::Begin("Terrain");                         // Create a window called "Hello, world!" and append into it.
			ImGui::PushItemWidth(100);
			ImGui::InputText("export name", saveName, IM_ARRAYSIZE(saveName));

			if(ImGui::CollapsingHeader("Presets"))
			{
				if(ImGui::Button("To Default"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
				{
					strcpy(saveName, "default");
					sizeWidth = 500;
					sizeHeight = 500;
					posMin = Eigen::Vector2d(-25, -25);
					posMax = Eigen::Vector2d(25, 25);
					t_noise._amplitude = 5.0;
					t_noise._base_freq = 1.0 / 200.0;
					t_noise._octaves = 8;
				}
			}

			if(ImGui::CollapsingHeader("Caracteristics"))
			{
				ImGui::InputInt("Width", &sizeWidth);
				ImGui::InputInt("Height", &sizeHeight);
				ImGui::InputDouble("minX", &posMin(0));
				ImGui::SameLine();
				ImGui::InputDouble("minY", &posMin(1));
				ImGui::InputDouble("maxX", &posMax(0));
				ImGui::SameLine();
				ImGui::InputDouble("maxY", &posMax(1));
				ImGui::InputDouble("Amplitude max", &t_noise._amplitude);
				ImGui::InputDouble("Base frequency", &t_noise._base_freq);
				ImGui::InputInt("Nb iterations", &t_noise._octaves);

				if(ImGui::Button("Generate"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
				{
					//ScalarField sf(sizeWidth, sizeHeight, posMin, posMax);
					mlm = MultiLayerMap(sizeWidth, sizeHeight, posMin, posMax);
					mlm.new_field();

					for(int j = 0; j < sizeHeight; ++j)
					{
						for(int i = 0; i < sizeWidth; i++)
						{
							mlm.get_field(0).at(i, j) = t_noise.get_noise(i, j);
						}
					}
				}
			}

			if(ImGui::CollapsingHeader("Operations"))
			{
				if(ImGui::Button("Generate"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
				{
					ScalarField sf(sizeWidth, sizeHeight, posMin, posMax);

					for(int j = 0; j < sizeHeight; ++j)
					{
						for(int i = 0; i < sizeWidth; i++)
						{
							sf.at(i, j) = t_noise.get_noise(i, j);
						}
					}

					sf.export_as_obj(saveName);
				}
			}

			if(ImGui::Button("Export as obj"))
			{
				mlm.generate_field().export_as_obj(std::string(saveName) + ".obj");
			}

			if(ImGui::Button("Export as pgm"))
			{
				mlm.generate_field().export_as_pgm(std::string(saveName) + ".pgm");
			}

			ImGui::End();
		}
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

