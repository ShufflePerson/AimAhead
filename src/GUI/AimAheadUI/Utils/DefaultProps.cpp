#include "../AimAheadUI.h"


ImGui::AH_Checkbox_Prop aimahead_ui::get_default_checkbox_prop() {
	ImGui::AH_Checkbox_Prop checkbox_prop;
	checkbox_prop.primary_font = get_font(EFont::FONT_MEDIUM);
	checkbox_prop.f_available_x_space = menu_settings.f_column_size - (menu_settings.f_box_margin * 2);
	return checkbox_prop;
}

ImGui::AH_Slider_Prop aimahead_ui::get_default_slider_prop() {
	ImGui::AH_Slider_Prop slider_prop;
	slider_prop.f_available_x_space = menu_settings.f_column_size - (menu_settings.f_box_margin * 2);
	slider_prop.primary_font = get_font(EFont::FONT_MEDIUM);
	slider_prop.circle_color = IM_COL32(166, 74, 201, 255);
	slider_prop.unselected_color = IM_COL32(82, 82, 82, 255);
	slider_prop.selected_color = IM_COL32(118, 56, 142, 255);
	return slider_prop;
}

ImGui::AH_ButtonInfo_Prop aimahead_ui::get_default_buttoninfo_prop() {
	ImGui::AH_ButtonInfo_Prop buttoninfo_prop;
	buttoninfo_prop.f_available_x_space = menu_settings.f_column_size - (menu_settings.f_box_margin * 2);
	buttoninfo_prop.primary_font = get_font(EFont::FONT_MEDIUM);
	buttoninfo_prop.button_background = IM_COL32(118, 56, 142, 255);
	return buttoninfo_prop;
}