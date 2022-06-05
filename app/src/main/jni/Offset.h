#ifndef Offset_H
#define Offset_H
#define LibName(Offset) KittyMemory::getAbsoluteAddress("liblogic.so", Offset)

struct {
	
    // class BattleManager - m_ShowPlayers
    uintptr_t ShowPlayer = base64(OBFUSCATE_KEY("0x14", '-'));
	
	// class ShowEntity - LogicFighter _logicFighter
    uintptr_t Logic_F = base64(OBFUSCATE_KEY("0x17C", '-'));
	
	// class ShowPlayer - CData_Hero_Element m_Config
    uintptr_t CData = base64(OBFUSCATE_KEY("0x430", '-'));
	
	// class BehaviorNode - int m_ID
    uintptr_t Id = base64(OBFUSCATE_KEY("0xC", '-'));
	
	// class LogicFighter - LogicSkillComp m_SkillComp
    uintptr_t Logic_SC = base64(OBFUSCATE_KEY("0x16C", '-'));
	
	// class LogicSkillComp - CoolDownComp m_CoolDownComp
    uintptr_t Cool_DC = base64(OBFUSCATE_KEY("0x2C", '-'));
	
	// class CData_AttrbuteDescribe_Element - string m_mName
    uintptr_t H_Name = base64(OBFUSCATE_KEY("0x10", '-'));
	
	// class ShowEntity - string m_RoleName
    uintptr_t U_Name = base64(OBFUSCATE_KEY("0x1A0", '-'));
    
	// class BattleManager - public void Update() { }
    uintptr_t Update = base64(OBFUSCATE_KEY("0x527E85C", '!'));
	
	// class LogicFighter - public virtual void LogicUpdate(int iTimeDelta) { }
	uintptr_t DroneHor = base64(OBFUSCATE_KEY("0x1E1EF58", '!'));
	
} Offset;

// class ShowEntity
// private LogicBattleManager get_logicBattleMgr() { }
static void *get_Logic(void *instance) {
    void *(*_get_L)(void *i) = (void *(*)(void *))getRealOffset(
    base64(OBFUSCATE_KEY("0x5301134", '-')));
    return _get_L(instance);
}

// class LogicBattleManager
// public EntityCampType GetCampType(LogicFighter entity, bool bEniemy = True) { }
static int GetCampType(void *thiz,void *entity, bool cek){
	uintptr_t (*_getId)(void *,void*,bool) = (uintptr_t (*)(void *,void*,bool))getRealOffset(
    base64(OBFUSCATE_KEY("0x1707C5C", '-')));
    return _getId(thiz, entity, cek);
}

// class LogicBattleManager
// public LogicPlayer GetPlayerRealSelf() { }
void *GetPlayerRealSelf(void *thiz) {
    void *(*_GetRealSelf)(void *nul) = (void *(*)(void *))getRealOffset(
    base64(OBFUSCATE_KEY("0x1713A30", '-')));
    return _GetRealSelf(thiz);
}

// class MCLogicFighter
// public int GetTotalHp() { }
static int GetHp(void *thiz) {
    int (*_GetHP)(void *players) = (int (*)(void *))getRealOffset(
    base64(OBFUSCATE_KEY("0x1F901DC", '-')));
   	return _GetHP(thiz);
}

// class MCLogicFighter
// public int GetTotalMaxHp() { }
static int GetMaxHp(void *thiz) {
    int (*_GetMaxHp)(void *players) = (int (*)(void *))getRealOffset(
    base64(OBFUSCATE_KEY("0x1F901A0", '-')));
   	return _GetMaxHp(thiz);
}

// class LogicFighter
// public bool GetHPEmpty() { }
static bool GetHPEmpty(void *instance) {
	bool (*_HP)(void *nul) = (bool (*)(void *))getRealOffset(
    base64(OBFUSCATE_KEY("0x1E12CC4", '-')));
    return _HP(instance);
}

// class LogicFighter
// public virtual Vector3 get_Position() { }
static Vector3 get_position(void *thiz) {
    Vector3 (*_GetPosition)(void *players) = (Vector3 (*)(void *))getRealOffset(
    base64(OBFUSCATE_KEY("0x1E37F98", '-')));
   	return _GetPosition(thiz);
}

// class LogicFighter
// public bool IsCoolDown(int spellId) { }
static bool IsCoolDown(void *instance, int id) {
	bool (*_IsCoolDown)(void *nul, int ids) = (bool (*)(void *, int))getRealOffset(
    base64(OBFUSCATE_KEY("0x1E27CB8", '-')));
    return _IsCoolDown(instance, id);
}

// class CalcFunMgr
// public uint GetRealCoolDownTimeForShow(int spellId, bool bTranID = True) { }
static int CalcCoolUpDown(void *thiz,int spellID){
	uintptr_t (*_CalcCoolUpDown)(void *thz, int a, bool c) = (uintptr_t (*)(void *, int, bool))getRealOffset(
    base64(OBFUSCATE_KEY("0x2052780", '-')));
    return _CalcCoolUpDown(thiz, spellID, true);
}

// public static Camera get_main() { }
static void *get_main() {
    void *(*_Camera_main)(void *nul) = (void *(*)(void *))getRealOffset(
    base64(OBFUSCATE_KEY("0x3C34E78", '-')));
    return _Camera_main(NULL);
}

// public float get_fieldOfView() { }
static float camera_get_fov(){
	float (*_GetFov)(void *cam) = (float (*)(void *))getRealOffset(
    base64(OBFUSCATE_KEY("0x3C30D7C", '-')));
	return _GetFov(get_main());
}

// public void set_fieldOfView(float value) { }
static void *camera_set_fov(float val) {
    void *(*_Set_fov)(void *cam,float v) = (void *(*)(void *,float))getRealOffset(
    base64(OBFUSCATE_KEY("0x3C30DF4", '-')));
   	return _Set_fov(get_main(),val);
}

// public Vector3 WorldToScreenPoint(Vector3 position) { }
static Vector3 WorldToScreenPoint(Vector3 WorldPos) {
    Vector3 (*_WorldToScreenScene)(void* cam, Vector3 position) = (Vector3(*)(void*, Vector3))getRealOffset(
    base64(OBFUSCATE_KEY("0x3C347D0", '-')));
    return _WorldToScreenScene(get_main(), WorldPos);
}

#endif
