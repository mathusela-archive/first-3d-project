#ifndef mainFunctions_H
#define mainFunctions_H

#include <glad/glad.h>
#include <glfw/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>
#include <vector>
#include <fstream>
#include <string>



// -----------------------------------------------------------------------------------------



unsigned int create_texture(std::string imagePath, GLenum imageFormat) {

    stbi_set_flip_vertically_on_load(true);

    int width, height, colorChannelCount;
    unsigned char *image = stbi_load(imagePath.c_str(), &width, &height, &colorChannelCount, 0);

    #ifdef DEV
        if (!image) {
            std::cout << "FAILED TO LOAD IMAGE" << std::endl;
        }
    #endif

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image);

    return texture;

}



// -----------------------------------------------------------------------------------------



template <class T>
std::vector<T> readCSV(std::string filePath) {

    std::ifstream csvInput(filePath);
    std::string fileText = "";
    std::string noSpaces;

    for(std::string line; getline(csvInput, line);) {
        fileText = fileText + line;
    }

    for(int i=0; i <= fileText.length(); i++) {
        if (fileText[i] != ' ') {
            noSpaces += fileText[i];
        }
    }

    std::vector<T> output;

    size_t pos = 0;
    std::string token;
    while ((pos = noSpaces.find(",")) != std::string::npos) {
        token = noSpaces.substr(0, pos);
        output.push_back((T)std::stof(token));
        noSpaces.erase(0, pos + 1);
    }
    output.push_back((T)std::stof(noSpaces));

    csvInput.close();
    csvInput.clear();

    return output;

}



// -----------------------------------------------------------------------------------------



std::string import_shader_source(std::string inputPath) {

    std::ifstream input(inputPath);
    std::string inputText = "";

    for(std::string line; getline(input, line);) {
        inputText = inputText + "\n" + line;
    }

    inputText += "\0";

    input.close();
    input.clear();

    return inputText;

}


unsigned int create_shader(std::string vertexShaderPath, std::string fragmentShaderPath) {

    std::string vertexShaderSourceString = import_shader_source(vertexShaderPath);
    const char* vertexShaderSource = vertexShaderSourceString.c_str();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    #ifdef DEV
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            std::cout << vertexShaderSource << "\n" << std::endl;
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR VERTEX SHADER COMPILATION FAILED\n" << infoLog << std::endl;
        };
    #endif

    std::string fragmentShaderSouceString = import_shader_source(fragmentShaderPath);
    const char* fragmentShaderSource = fragmentShaderSouceString.c_str();
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    #ifdef DEV
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            std::cout << fragmentShaderSource << "\n" << std::endl;
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR FRAGMENT SHADER COMPILATION FAILED\n" << infoLog << std::endl;
        };
    #endif

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;

}



// -----------------------------------------------------------------------------------------



#endif