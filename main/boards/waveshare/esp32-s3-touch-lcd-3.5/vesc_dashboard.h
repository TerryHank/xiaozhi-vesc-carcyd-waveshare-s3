#pragma once
#include <functional>
#include "display.h"

// Embeds CarCYD's original UI while retaining Xiaozhi's conversation widgets.
class VescDashboard {
public:
    VescDashboard(Display* display, lv_obj_t* container, lv_obj_t* chat, lv_obj_t* icons,
                  lv_obj_t* state);
    void Resize(int width, int height);
    void SetMessage(const char* role, const char* message);
    void ShowChat(bool show);
    void SetChatFont(const lv_font_t* font);
    lv_obj_t* ChatOverlay() const { return chat_layer_; }
    bool IsChatPage() const { return chat_open_; }
    void SetPageChanged(std::function<void(bool)> callback) { page_changed_ = std::move(callback); }

private:
    void Refresh();
    lv_obj_t* chat_layer_;
    lv_obj_t* chat_;
    lv_obj_t* state_;
    lv_obj_t* status_;
    bool chat_open_ = false;
    std::function<void(bool)> page_changed_;
};
