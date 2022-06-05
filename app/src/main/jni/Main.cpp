#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include "Unity/Quaternion.hpp"
#include "Unity/Vector3.hpp"
#include "Unity/Color.hpp"
#include "Unity/Vector2.hpp"
#include "Unity/Unity.h"
#include "Unity/Rect.hpp"
#include "Unity/Esp.h"
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"
#include "KittyMemory/MemoryPatch.h"
#include "Menu.h"
#include "Offset.h"
#include "Unity/MonoString.h"
#include <Substrate/SubstrateHook.h>
#include <Substrate/CydiaSubstrate.h>
#define targetLibName OBFUSCATE("liblogic.so")
#include "Includes/Macros.h"

//****** Project By: @njt *****\\

float Org_Fov;
float FovValue;
bool UseFOV = false;
bool ESPLine = false;
bool ESPBox = false;
bool ESPName = false;
bool ESPCircle = false;
bool ESPCooldown= false;
int SizeLine = 2;
int Size = 35;
int SizeTxt = 20;
int Cooldown1 = 0;
int Cooldown2 = 0;
int Cooldown3 = 0;

struct PlayerData {
    Vector3 Location;
	int HP;
	int MAXhp;
	MonoString *EnemyNAME;
	MonoString *HeroName;
	int skil[5];
	int timer[5];
	bool cekF[5];
};

struct Response {
    int PlayerCount;
    PlayerData Players[10];
    Vector3 LocalROOT;
};
Response resp;
char StringIO[69];

extern "C" {
JNIEXPORT jobjectArray
JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_getFeatureList(JNIEnv *env, jobject context) { jobjectArray ret;
    MakeToast(env, context, OBFUSCATE("Modded by 中〆DreamXD"), Toast::LENGTH_LONG);
    const char *features[] = {
        OBFUSCATE("Category_Safe Hack"),
        OBFUSCATE("1_Toggle_ESP Player Line"),
        OBFUSCATE("2_Toggle_ESP Player Box"),
        OBFUSCATE("3_Toggle_ESP Player Name"),
		OBFUSCATE("4_Toggle_ESP Player Circle"),
		OBFUSCATE("5_Toggle_ESP Player Cooldown"),
		OBFUSCATE("6_SeekBar_Line & Box Size _1_7"),
		OBFUSCATE("7_SeekBar_Camera Width _0_30"),
    };

    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
    env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
    env->NewStringUTF(""));
    for (int i = 0; i < Total_Feature; i++)
    env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));
    pthread_t ptid;
    pthread_create(&ptid, NULL, antiLeech, NULL);
    return (ret);
}

JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_Preferences_Changes(JNIEnv *env, jclass clazz, jobject obj,
    jint featNum, jstring featName, jint value,
    jboolean boolean, jstring str) {
    LOGD(OBFUSCATE("Feature name: %d - %s | Value: = %d | Bool: = %d | Text: = %s"), featNum,
    env->GetStringUTFChars(featName, 0), value,
    boolean, str != NULL ? env->GetStringUTFChars(str, 0) : "");
		 
    switch (featNum) {
        case 1:
            ESPLine = boolean;
            break;
        case 2:
            ESPBox = boolean;
            break;
		case 3:
            ESPName = boolean;
            break;
		case 4:
		    ESPCircle = boolean;
			break;
		case 5:
            ESPCooldown = boolean;
            break;
	    case 6:
            SizeLine = value;
            break;
		case 7:
            FovValue = value;
		    UseFOV = true;
            break;
    }
  }
}


//***** HOOKING STRING *****\\

void (*DroneHor)(void* thiz,int td);
void _DroneHor(void *thiz,int td){
		if(thiz != NULL){
			if(Org_Fov == 0){
			Org_Fov = camera_get_fov();
			}
			if(FovValue > 0 && Org_Fov != 0){
				if(UseFOV){		
					camera_set_fov((float)Org_Fov + FovValue);
					UseFOV = false;
				}
			}
			if(FovValue == 0 && Org_Fov != 0){
			    camera_set_fov((float)Org_Fov);
			}
		}
	DroneHor(thiz,td);
}

void (*old_Game_Update)(void *instance);
void Game_Update(void *instance) {
    if(instance != NULL) {
	    if (ESPLine || ESPBox || ESPName || ESPCircle || ESPCooldown) {
        monoList < void **> *EntityList = *(monoList < void **> **)((long)instance + Offset.ShowPlayer);
        if(EntityList != NULL) {
		   void *one_entity = EntityList->getItems()[1];
		   void *logicBattleMgr = get_Logic(one_entity);
		   void *selfPlayer = GetPlayerRealSelf(logicBattleMgr);
		   auto lock = get_position(selfPlayer);
	       resp.LocalROOT = WorldToScreenPoint(lock);
		   if(selfPlayer != NULL && logicBattleMgr != NULL) {
              int playerCount = EntityList->getSize();
		      resp.PlayerCount = playerCount;
              for (int u = 0; u < playerCount; u++) {
		           void *entity = EntityList->getItems()[u];
			       void *logicFighter = *(void **)((long) entity + Offset.Logic_F);
			       int selfCamp = GetCampType(logicBattleMgr,selfPlayer,false);
			       int enCamp =  GetCampType(logicBattleMgr,logicFighter,false);
			       if (selfCamp != enCamp) {
					   if (!GetHPEmpty(logicFighter)) {
                            auto enmPos = get_position(logicFighter);
                            auto enemyPos = WorldToScreenPoint(enmPos);
							void *CData_hero = *(void **)((uintptr_t) entity + Offset.CData);
							if (CData_hero != NULL){
								int ID = *(int *)((uintptr_t) CData_hero + Offset.Id);
								void *m_SkillComp = *(void **)((long) logicFighter + Offset.Logic_SC);
								void *m_CoolDownComp = *(void **)((long) m_SkillComp + Offset.Cool_DC);
								int _Skil1 = 100 * ID + 10;
								int _Skil2 = 100 * ID + 20;
								int _Skil3 = 100 * ID + 30;
								if (!IsCoolDown(logicFighter, _Skil1)){
									if (!resp.Players[u].cekF[1]) {
										int skl1 = CalcCoolUpDown(m_CoolDownComp, _Skil1);
									    Cooldown1 = skl1 / 1000;
									    resp.Players[u].skil[1] = Cooldown1;
										resp.Players[u].cekF[1] = true;
									} else if (resp.Players[u].cekF[1]) {
										if (resp.Players[u].timer[1] >= 50) {
											resp.Players[u].skil[1]--;
											resp.Players[u].timer[1] = 0;
										}
										resp.Players[u].timer[1]++;
									}
								} else {
									resp.Players[u].timer[1] = 0;
									resp.Players[u].cekF[1] = false;
									resp.Players[u].skil[1] = 0;
								}
								if (!IsCoolDown(logicFighter, _Skil2)){
									if (!resp.Players[u].cekF[2]) {
										int skl2 = CalcCoolUpDown(m_CoolDownComp, _Skil2);
									    Cooldown2 = skl2 / 1000;
									    resp.Players[u].skil[2] = Cooldown2;
										resp.Players[u].cekF[2] = true;
									} else if (resp.Players[u].cekF[2]) {
										if (resp.Players[u].timer[2] >= 50) {
											resp.Players[u].skil[2]--;
											resp.Players[u].timer[2] = 0;
										}
										resp.Players[u].timer[2]++;
									}
								} else {
									resp.Players[u].timer[2] = 0;
									resp.Players[u].cekF[2] = false;
									resp.Players[u].skil[2] = 0;
								}
								if (!IsCoolDown(logicFighter, _Skil3)){
									if (!resp.Players[u].cekF[3]) {
										int skl3 = CalcCoolUpDown(m_CoolDownComp, _Skil3);
									    Cooldown3 = skl3 / 1000;
									    resp.Players[u].skil[3] = Cooldown3;
										resp.Players[u].cekF[3] = true;
									} else if (resp.Players[u].cekF[3]) {
										if (resp.Players[u].timer[3] >= 50) {
											resp.Players[u].skil[3]--;
											resp.Players[u].timer[3] = 0;
										}
										resp.Players[u].timer[3]++;
									}
								} else {
									resp.Players[u].timer[3] = 0;
									resp.Players[u].cekF[3] = false;
									resp.Players[u].skil[3] = 0;
								}
								MonoString *CDATA = *(MonoString **)((uintptr_t) CData_hero + Offset.H_Name);
							    resp.Players[u].HeroName = CDATA;
							}
							MonoString *USR_NAME = *(MonoString **)((long) entity + Offset.U_Name);
							resp.Players[u].EnemyNAME = USR_NAME;
							int hpPlayer = GetHp(logicFighter);
						    int hpMaxPlayer = GetMaxHp(logicFighter);
						    resp.Players[u].MAXhp = hpMaxPlayer;
						    resp.Players[u].HP = hpPlayer;
                            resp.Players[u].Location = enemyPos;
                         } else {
                            resp.Players[u].Location = Vector3::Zero();
                         }
			         } else {
				         resp.Players[u].EnemyNAME = NULL;
			         }
                 }
		     }
          }
	  }
  }
  old_Game_Update(instance);
}

extern "C"{
void DrawESP(ESP esp, int screenWidth, int screenHeight) {
	Vector2 Type = Vector2(screenWidth /2, screenHeight /1 - 10);
    Vector2 screen(screenWidth, screenHeight);
    if (ESPLine || ESPBox || ESPName || ESPCircle || ESPCooldown) {
        int playerCount = resp.PlayerCount;
        auto _localROOT = resp.LocalROOT;
        for (int u = 0; u < playerCount; u++) {
             auto enemyPos = resp.Players[u].Location;
			 MonoString *_EnemyNick = resp.Players[u].EnemyNAME;
			 MonoString *_HeroName = resp.Players[u].HeroName;
             if (enemyPos != Vector3::Zero()) {
                Vector2 From = Vector2(10, screenHeight + 100); //100
                if (_localROOT.Z > 0.0f){ //0.0f
                    From = Vector2(_localROOT.X + 100, screen.Y - _localROOT.Y - 50); //100
                }
                auto To = Vector2(enemyPos.X + 100, screen.Y - enemyPos.Y - 50); //100-75
                if (enemyPos.Z > 0.0f) { ///0.0f
				    if (ESPLine){
                        esp.DrawLine(Color(255, 255, 255, 125), SizeLine, From, To);
                    }
                    if (ESPBox){
                        Vector2 v1 = Vector2(To.X - Size, To.Y - 35 - Size);
                        Vector2 v2 = Vector2(To.X + Size, To.Y - 35 - Size);
                        Vector2 v3 = Vector2(To.X + Size, To.Y - 35 + Size);
                        Vector2 v4 = Vector2(To.X - Size, To.Y - 35 + Size);
                        
                        esp.DrawLine(Color(255, 255, 255, 125), SizeLine, v1, v2);
                        esp.DrawLine(Color(255, 255, 255, 125), SizeLine, v2, v3);
                        esp.DrawLine(Color(255, 255, 255, 125), SizeLine, v3, v4);
                        esp.DrawLine(Color(255, 255, 255, 125), SizeLine, v4, v1);
                    }
					if (ESPName){
						char nick[0xFF] = {0};
						char nick2[0xFF] = {0};
	                    if(_EnemyNick != NULL) {
							snprintf(nick2,sizeof(nick2), OBFUSCATE("%s"), _HeroName->toChars());
							snprintf(nick,sizeof(nick), OBFUSCATE("%s"), _EnemyNick->toChars());
							esp.DrawNameRB(To, nick, nick2, Size, Size + Size, SizeTxt, 1);
						}
		            }
					if (ESPCircle) {
                        esp.DrawCircle(Color::white(), 10, From, 1);
						esp.DrawCircle(Color::white(), 10, To, 1);
                    }
					if (ESPCooldown) {
						int skil1 = resp.Players[u].skil[1];
						int skil2 = resp.Players[u].skil[2];
						int skil3 = resp.Players[u].skil[3];
						char skl1[10];
						char skl2[10];
						char skl3[10];
						if (skil1 <= 0){
							sprintf(skl1, OBFUSCATE(" 🔶 ⇛"), skil1);
						} else if (skil1 >= 1) {
							sprintf(skl1, OBFUSCATE(" %d "), skil1);
						}
						if (skil2 <= 0){
							sprintf(skl2, OBFUSCATE(" 🔶 ⇛"), skil2);
						} else if (skil2 >= 1) {
							sprintf(skl2, OBFUSCATE(" %d "), skil2);
						}
						if (skil3 <= 0){
							sprintf(skl3, OBFUSCATE(" 🔶 "), skil3);
						} else if (skil3 >= 1) {
							sprintf(skl3, OBFUSCATE(" %d "), skil3);
						}
						int width = Size * 4;
						sprintf(StringIO, OBFUSCATE("%s %s %s"), skl1, skl2, skl3);
						esp.DrawCD(To, StringIO, width, SizeTxt);
					}
                }
            }
        }
	}
}
	 
ESP espOverlay;
JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_Espview(JNIEnv *env, jclass type, jobject espView, jobject canvas) {
    espOverlay = ESP(env, espView, canvas);
    if (espOverlay.isValid()) {
        DrawESP(espOverlay, espOverlay.getWidth(), espOverlay.getHeight());
    }
  }
}

void *hack_thread(void *) {
    LOGI(OBFUSCATE("pthread created"));
    do {
        sleep(20);
    } while (!isLibraryLoaded(targetLibName));
    LOGI(OBFUSCATE("%s has been loaded"), (const char *) targetLibName);
	
	MSHookFunction((void*)getRealOffset(Offset.Update), (void *)Game_Update, (void **) &old_Game_Update);
	MSHookFunction((void*)getRealOffset(Offset.DroneHor), (void *)_DroneHor, (void **) &DroneHor);

    LOGI(OBFUSCATE("Done"));
    return NULL;
}

__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}

