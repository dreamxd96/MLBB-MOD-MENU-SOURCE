#ifndef ESP_H
#define ESP_H


#include <jni.h>


class ESP {
private:
    JNIEnv *_env;
    jobject _cvsView;
    jobject _cvs;

public:
    ESP() {
        _env = nullptr;
        _cvsView = nullptr;
        _cvs = nullptr;
    }

    ESP(JNIEnv *env, jobject cvsView, jobject cvs) {
        this->_env = env;
        this->_cvsView = cvsView;
        this->_cvs = cvs;
    }

    bool isValid() const {
        return (_env != nullptr && _cvsView != nullptr && _cvs != nullptr);
    }

    int getWidth() const {
        if (isValid()) {
            jclass canvas = _env->GetObjectClass(_cvs);
            jmethodID width = _env->GetMethodID(canvas, "getWidth", "()I");
            return _env->CallIntMethod(_cvs, width);
        }
        return 0;
    }

    int getHeight() const {
        if (isValid()) {
            jclass canvas = _env->GetObjectClass(_cvs);
            jmethodID width = _env->GetMethodID(canvas, "getHeight", "()I");
            return _env->CallIntMethod(_cvs, width);
        }
        return 0;
    }

    void
    DrawLine(Color color, float thickness, Vector2 start, Vector2 end) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawline = _env->GetMethodID(canvasView, "DrawLine",
                                                   "(Landroid/graphics/Canvas;IIIIFFFFF)V");
            _env->CallVoidMethod(_cvsView, drawline, _cvs, (int) color.A, (int) color.R,
                                 (int) color.G, (int) color.B,
                                 thickness,
                                 start.X, start.Y, end.X, end.Y);
        }
    }

    void DrawText(Color color, const char *txt, Vector2 pos, float size) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawtext = _env->GetMethodID(canvasView, "DrawText",
                                                    "(Landroid/graphics/Canvas;IIIILjava/lang/String;FFF)V");
            _env->CallVoidMethod(_cvsView, drawtext, _cvs, (int) color.A, (int) color.R,
                                 (int) color.G, (int) color.B,
                                 _env->NewStringUTF(txt), pos.X, pos.Y, size);
        }
    }
	
	void DrawName(Color color, const char *txt, Vector2 pos, float size) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawname = _env->GetMethodID(canvasView, "DrawName",
                                                    "(Landroid/graphics/Canvas;IIIILjava/lang/String;FFF)V");
			jstring s=_env->NewStringUTF(txt);
            _env->CallVoidMethod(_cvsView, drawname, _cvs, (int) color.A, (int) color.R,
                                 (int) color.G, (int) color.B,
                                 s, pos.X, pos.Y, size);
			_env->DeleteLocalRef(s);
        }
    }

    void DrawCircle(Color color,float stroke, Vector2 pos, float radius) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawcircle = _env->GetMethodID(canvasView, "DrawCircle",
                                                     "(Landroid/graphics/Canvas;IIIIFFFF)V");
            _env->CallVoidMethod(_cvsView, drawcircle, _cvs, (int) color.A, (int) color.R,
                                 (int) color.G, (int) color.B,stroke, pos.X, pos.Y, radius);
        }
    }
    void DrawCircle1(Color color, Vector2 pos, float radius) {
        if (isValid()) {
            jclass canvasView = _env->GetObjectClass(_cvsView);
            jmethodID drawfilledcircle = _env->GetMethodID(canvasView, "DrawFilledCircle",
                                                           "(Landroid/graphics/Canvas;IIIIFFF)V");
            _env->CallVoidMethod(_cvsView, drawfilledcircle, _cvs, (int) color.A, (int) color.R,
                                 (int) color.G, (int) color.B, pos.X, pos.Y, radius);
        }
    }
    void DrawBox(Color color, float stroke, Rect rect) {
        Vector2 v1 = Vector2(rect.m_XMin, rect.m_YMin);
        Vector2 v2 = Vector2(rect.m_XMin + rect.m_Width, rect.m_YMin);
        Vector2 v3 = Vector2(rect.m_XMin + rect.m_Width, rect.m_YMin + rect.m_Height);
        Vector2 v4 = Vector2(rect.m_XMin, rect.m_YMin + rect.m_Height);

        DrawLine(color, stroke, v1, v2); // ALINHAR
        DrawLine(color, stroke, v2, v3); // LINHA DIREITA
        DrawLine(color, stroke, v3, v4); // LINHA ABAIXO
        DrawLine(color, stroke, v4, v1); // LINHA ESQUERDA
    }


    void DrawHorizontalHealthBar(Vector2 screenPos, float width, float maxHealth, float currentHealth) {
        
		Color clr = Color(255, 255, 255, 200);
        float hpWidth = (currentHealth * width) / maxHealth;
        if (currentHealth <= (maxHealth * 0.6)) {
            clr = Color(255, 255, 255, 200);
        }
        if (currentHealth < (maxHealth * 0.3)) {
            clr = Color(255, 255, 255, 200);
        }
		float WD = width / 2;
		
        DrawBox(clr, 1, Rect(screenPos.X - WD, screenPos.Y - 15, width + 2, 15));
		Vector2 FK = Vector2(screenPos.X - WD + hpWidth, screenPos.Y + 15 / 2 - 15);
		Vector2 MK = Vector2(screenPos.X - WD - 1, screenPos.Y + 15 / 2 - 15);
		DrawLine(Color(250, 69, 1, 210), 14, MK, FK);
        DrawBox(clr, 1, Rect(screenPos.X - WD, screenPos.Y - 15, hpWidth, 15));
    }

    void DrawCrosshair(Color clr, Vector2 center, float size, float thickness) {
        float x = center.X - (size / 2.0f);
        float y = center.Y - (size / 2.0f);
        DrawLine(clr, thickness, Vector2(x, center.Y), Vector2(x + size, center.Y));
        DrawLine(clr, thickness, Vector2(center.X, y), Vector2(center.X, y + size));
    }
	
	void DrawNameRB(Vector2 center, char *txt, char *txt2, float glith, float horizontal, float size, int type) {
		Vector2 To = Vector2(center.X + glith, center.Y + glith);
		Vector2 To2 = Vector2(To.X + horizontal, To.Y);
		
		float ab = horizontal / 2;
		Vector2 Lock = Vector2(To.X + ab + 10, To.Y - 5);
		Vector2 Lock2 = Vector2(To.X + ab + 10, To.Y + size);
		
		DrawLine(Color(255, 255, 255, 125), 2, center, To);
		DrawLine(Color(255, 255, 255, 125), 2, To, To2);
		
		if (type == 0){
			DrawText(Color::yellowRGB(200), txt, Lock, size);
		} else if (type == 1){
			DrawName(Color::yellowRGB(200), txt2, Lock, size);
		} else if (type == 2){
			DrawText(Color::yellowRGB(200), txt, Lock, size);
			DrawName(Color::yellowRGB(200), txt2, Lock2, size);
		}
	}
	
	void DrawNameLT(Vector2 center, float glith, float horizontal, int width, int maxHP, int HP) {
		Vector2 To = Vector2(center.X + glith, center.Y - glith);
		Vector2 To2 = Vector2(To.X + horizontal, To.Y);
		
		float ab = 50;
		int hpWidth = (HP * width) / maxHP;
		char hpC [] = "%";
		char hpPer[10];
		sprintf(hpPer, "%d %s", hpWidth, hpC);
		
		Vector2 Lvl2 = Vector2(To.X + ab, To.Y - 7);
		Vector2 Lock = Vector2(To.X + ab, To.Y - 5);
		
		DrawLine(Color(255, 255, 255, 125), 2, center, To);
		DrawLine(Color(255, 255, 255, 125), 2, To, To2);
		
		DrawHorizontalHealthBar(Lock, width, maxHP, HP);
		DrawText(Color::yellowRGB(200), hpPer, Lvl2, 15);
	}
	
	void DrawCD(Vector2 center, char *txt, float width, float size){
		Vector2 To = Vector2(center.X + width, center.Y);
		float m_final = width / 2 + 20;
		Vector2 Lock = Vector2(center.X + m_final, center.Y - 5);
		
		DrawLine(Color(255, 255, 255, 125), 2, center, To);
		
		DrawName(Color::yellowRGB(225), txt, Lock, size);
	}
};

#endif
