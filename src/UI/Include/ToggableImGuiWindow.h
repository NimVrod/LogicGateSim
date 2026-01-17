#pragma once

class ToggableImGuiWindow {
public:
    virtual ~ToggableImGuiWindow() = default;

    void show() { visible = true; }
    void hide() { visible = false; }
    void toggle() { visible = !visible; }
    bool isVisible() const { return visible; }

    virtual void renderUI() = 0;

protected:
    bool visible = false;
};