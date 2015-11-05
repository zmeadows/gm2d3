#ifndef GM2D3_AUTO_CONTROLLER
#define GM2D3_AUTO_CONTROLLER

#include <Fl/Fl_Input.H>
#include <Fl/Fl_Pack.H>
#include <Fl/Fl_Button.H>
#include <Fl/Fl_Light_Button.H>

class GM2D3AutoController : public Fl_Input {
    public:
        GM2D3AutoController(int x, int y, int w, int h, const char* label);
        virtual ~GM2D3AutoController();
};

class GM2D3AutoControllerGroup : public Fl_Pack {
    public:
        GM2D3AutoControllerGroup(int x, int y, int w, int h, const char* label);
        virtual ~GM2D3AutoControllerGroup();

    private:
        GM2D3AutoController *aca, *vca, *rca;
        Fl_Light_Button *calibrate_button, *go_button;
        Fl_Button *stop_button;
};

#endif
