
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene


    SDL_Event e;
    bool quit = false;
    //OPENGL INITIALIZE
    std::vector<neu::vec3> points{ { -0.5f, -0.5f, 0 }, { 0, 0.5f, 0 }, { 0.5f, -0.5f, 0 } };
    std::vector<neu::vec3> colors{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
    std::vector<neu::vec2> texcoord{ {0,0},{0.5f,1.0f},{1,1} };

    struct Vertex {
        neu::vec3 position;
        neu::vec3 color;
        neu::vec2 texcoord;
    };

    std::vector<Vertex> vertices{
            { { -0.5f, -0.5f, 0.0f }, {1,0,0}, {0,0} },
            { { -0.5f,  0.5f, 0.0f }, {0,1,0}, {0,1} },
            { {  0.5f,  0.5f, 0.0f }, {0,0,1}, {1,1} },
            { {  0.5f, -0.5f, 0.0f }, {0,0,1}, {1,0}}
    };
    std::vector<GLuint> indices{0,1,2,2,3,0};
    //vertex buffer
    GLuint vbo;
    glGenBuffers( 1, &vbo );

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* vertices.size(), vertices.data(), GL_STATIC_DRAW);

    //indexbuffer
    GLuint ibo;
    glGenBuffers(1, &ibo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* indices.size(), indices.data(), GL_STATIC_DRAW);

    //vertex array
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));


    /*
    GLuint vbo[3];
    glGenBuffers(3, vbo);

	//vertex buffer(positions)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * points.size(), points.data(), GL_STATIC_DRAW);

    //vertex buffer(color)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * colors.size(), colors.data(), GL_STATIC_DRAW);

    //vertex buffer(texcoord)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec2) * texcoord.size(), texcoord.data(), GL_STATIC_DRAW);


    //vertex array
    GLuint vao;
	glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //position
	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //color
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //texcoord
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    */




    //vertex shader
    auto vs = neu::Resources().Get<neu::Shader>("shaders/basic.vert", GL_VERTEX_SHADER);

    //fragment shader
   /*std::string fs_source;
    neu::file::ReadTextFile("shaders/Basic.frag", fs_source);
    const char* fs_cstr = fs_source.c_str();
    GLuint fs;
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_cstr, NULL);
    glCompileShader(fs);
    auto fs = neu::Resources().Get<neu::Shader>("shaders/basic.frag", GL_FRAGMENT_SHADER);
    
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(fs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }
    */

    //Shader program link
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetProgramInfoLog(program, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }
    glUseProgram(program);

    //texture
    neu::res_t<neu::Texture> texture = neu::Resources().Get <neu::Texture>("textures/beast.png");

    //uniform
    GLint uniform = glGetUniformLocation(program, "u_time");
    
    GLuint tex_uniform = glGetUniformLocation(program, "u_texture");
    glUniform1i(tex_uniform, 0);


        // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // update
        neu::GetEngine().Update();

        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

        glUniform1f(uniform, neu::GetEngine().GetTime().GetTime());
      
        /*
        float angle = neu::GetEngine*/
       
        // draw
        //neu::vec3 color{ 0, 0, 0 };
        //neu::GetEngine().GetRenderer().SetColor(color.r, color.g, color.b);
        neu::GetEngine().GetRenderer().Clear();



        glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES,0,(GLsizei)points.size()); //remove commments later
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);


       glBegin(GL_TRIANGLES);
   

       int x;
       for (x = 0; points.size() > x; x++) {
           glColor3f(colors[x].r, colors[x].g, colors[x].b);

           glVertex3f(points[x].x, points[x].y, points[x].z);
       }

       glEnd();

      
        /*
        glColor3f(1,0,0);
        glVertex3f(-0.5f,-0.5f,0);
        glColor3f(0,1,0);
        glVertex3f(0, 0.5f,0);
        glColor3f(1,1,0);
        glVertex3f(.5f,-.5f,0);

        glEnd();
        */

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
