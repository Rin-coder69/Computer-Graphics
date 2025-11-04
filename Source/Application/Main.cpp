
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene
    auto scene = std::make_shared <neu::Scene>();
    scene->Load("Scenes/scene01.json");
    scene->Start();

    // MAIN LOOP
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // update
        neu::GetEngine().Update();
        float dt = neu::GetEngine().GetTime().GetDeltaTime();

        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;
   
        // draw
        neu::GetEngine().GetRenderer().Clear();

        //Start new new imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        //set ImGui
        ImGui::Begin("Editor");
        ImGui::Text("Hello World");
        ImGui::Text("Press 'ESC' to quit");
        ImGui::End();

        //draw ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        scene->Draw(neu::GetEngine().GetRenderer());
        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
