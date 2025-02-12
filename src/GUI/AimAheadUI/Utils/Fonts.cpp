#include "../AimAheadUI.h"


ImFont* font_thin;
ImFont* font_regular;
ImFont* font_medium;
ImFont* font_light;
ImFont* font_extrabold;
ImFont* font_bold;
ImFont* font_black;
void aimahead_ui::load_fonts() {
	float font_size = 32.0f;
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	std::filesystem::path font_dir = std::filesystem::absolute("./bin/fonts");
	ImFontConfig font_config;
	font_config.OversampleH = 1;
	font_config.OversampleV = 1;

	font_thin = io.Fonts->AddFontFromFileTTF((font_dir / "font_thin.ttf").string().c_str(), font_size);
	font_regular = io.Fonts->AddFontFromFileTTF((font_dir / "font_regular.ttf").string().c_str(), font_size);
	font_medium = io.Fonts->AddFontFromFileTTF((font_dir / "font_medium.ttf").string().c_str(), font_size, &font_config);
	font_light = io.Fonts->AddFontFromFileTTF((font_dir / "font_light.ttf").string().c_str(), font_size);
	font_extrabold = io.Fonts->AddFontFromFileTTF((font_dir / "font_extrabold.ttf").string().c_str(), font_size);
	font_bold = io.Fonts->AddFontFromFileTTF((font_dir / "font_bold.ttf").string().c_str(), font_size);
	font_black = io.Fonts->AddFontFromFileTTF((font_dir / "font_black.ttf").string().c_str(), font_size);

	io.Fonts->Build();

}


ImFont* aimahead_ui::get_font(EFont font_type) {
	switch (font_type)
	{
	case EFont::FONT_MEDIUM:
		return font_medium;
	default:
		return nullptr;
		break;
	}
}