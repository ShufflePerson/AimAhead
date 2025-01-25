#include "../GUI.h"




GLuint mat_to_texture(const cv::Mat& image) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLenum internalFormat = GL_RGB8;
    GLenum dataFormat = GL_BGR;
    if (image.channels() == 1) {
        internalFormat = GL_R8;
        dataFormat = GL_RED;
    }
    else if (image.channels() == 3) {
        internalFormat = GL_RGB8;
        dataFormat = GL_BGR;
    }
    else if (image.channels() == 4) {
        internalFormat = GL_RGBA8;
        dataFormat = GL_BGRA;
    }
    else {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &textureID);
        return 0;
    }

    if (image.depth() != CV_8U) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &textureID);
        return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.cols, image.rows, 0, dataFormat, GL_UNSIGNED_BYTE, image.data);

    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

void gui::__render__preview_tab__(AimConfig* config) {
	cv::Mat image = preview::get_cached_rendered_image();
    GLuint textureID = mat_to_texture(image);

	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::Columns(2, "Columns");
	
    
    ImGui::Text("TOP Aim position margin");
	ImGui::SliderInt("##margin_top", &config->i_head_margin, 0, 100);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) ImGui::SetTooltip("A percentege of a margin from the top of the box when TOP aim position is selected.");

	ImGui::NextColumn();
	ImGui::SetColumnWidth(0, 300.0f);
	ImGui::Image((void*)(intptr_t)textureID, ImVec2(image.cols / 1.45f, image.rows / 1.45f)); 
	ImGui::Columns(1);

}