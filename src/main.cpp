#include <iostream>
#include <ScalarField.hpp>
#include <Noise/TerrainNoise.hpp>

#ifdef __linux__
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
#endif


int main()
{
#ifdef __linux__
	char saveName[256] = "default.obj";
	int sizeWidth = 500;
	int sizeHeight = 500;
	Eigen::Vector2d posMin(-25, -25);
	Eigen::Vector2d posMax(25, 25);
	TerrainNoise t_noise(5.0, 1.0 / 200.0, 8);
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);

	if(!glfwInit())
	{
		return 1;
	}

	const char* glsl_version = "#version 330";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(600, 400, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);

	if(window == NULL)
	{
		return 1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
	bool err = glewInit() != GLEW_OK;

	if(err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	// Setup style
	ImGui::StyleColorsDark();
	bool show_demo_window = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Main loop
	while(!glfwWindowShouldClose(window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;
			ImGui::Begin("Configure Terrain");                          // Create a window called "Hello, world!" and append into it.
			ImGui::InputText("export name", saveName, IM_ARRAYSIZE(saveName));
			ImGui::InputInt("Width", &sizeWidth);
			ImGui::InputInt("Height", &sizeHeight);
			ImGui::InputDouble("minX", &posMin(0));
			ImGui::InputDouble("minY", &posMin(1));
			ImGui::InputDouble("maxX", &posMax(0));
			ImGui::InputDouble("maxY", &posMax(1));
			ImGui::InputDouble("Amplitude max", &t_noise._amplitude);
			ImGui::InputDouble("Base frequency", &t_noise._base_freq);
			ImGui::InputInt("Nb iterations", &t_noise._octaves);

			if(ImGui::Button("Generate"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
			{
				ScalarField sf(sizeWidth, sizeHeight, posMin, posMax);

				for(int j = 0; j < sizeHeight; ++j)
				{
					for(int i = 0; i < sizeWidth; i++)
					{
						sf.at(i, j) = t_noise.getNoise(i, j);
					}
				}

				sf.exportAsObj(saveName);
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
#endif
#ifdef __WIN32__
	std::cout << "yolo c'est rigolo" << std::endl;
#endif
	return 0;
}

