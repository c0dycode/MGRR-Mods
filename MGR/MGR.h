#pragma once
#include <Windows.h>
#include <cstdint>
#include <random>

struct Vector4 {
	float X;
	float Y;
	float Z;
	float A;
};


// Created with ReClass.NET by KN4CK3R

// PartIndexes
enum class EBodyParts : int32_t
{
	Chest = 1,
	Chest2 = 2,
	Chest3 = 3,
	FullBody = 0,
	Head = 4,
	Head2 = 5,
	LeftArm = 10,
	LeftArm2 = 11,
	LeftButt = 1302,
	LeftElbow = 1291,
	LeftFoot = 21,
	LeftHand = 13,
	LeftKneepad = 1300,
	LeftLeg = 19,
	LeftLowerArm = 12,
	LeftLowerArmShield = 1292,
	LeftLowerLeg = 20,
	LeftMiddleFinger = 519,
	LeftMiddleFinger2 = 520,
	LeftMiddleFinger3 = 521,
	LeftMiddleFinger4 = 522,
	LeftPinky = 527,
	LeftPinky2 = 528,
	LeftPinky3 = 529,
	LeftPinky4 = 530,
	LeftPointy = 515,
	LeftPointy2 = 516,
	LeftPointy3 = 517,
	LeftPointy4 = 518,
	LeftRingFinger = 523,
	LeftRingFinger2 = 524,
	LeftRingFinger3 = 525,
	LeftRingFinger4 = 526,
	LeftShoulder = 1289,
	LeftShoulderBlade = 1296,
	LeftShoulderTop = 1288,
	LeftSleeve = 1293,
	LeftThumb = 512,
	LeftThumb2 = 513,
	LeftThumb3 = 514,
	LeftToes = 22,
	LeftUpperArmShield = 1290,
	LeftUpperLeg = 1299,
	Neck = 1280,
	RightArm = 6,
	RightArm2 = 7,
	RightButt = 1301,
	RightElbow = 1284,
	RightFoot = 17,
	RightHand = 9,
	RightKneepad = 1298,
	RightLeg = 15,
	RightLowerArm = 8,
	RightLowerArmShield = 1285,
	RightLowerLeg = 16,
	RightMiddleFinger = 263,
	RightMiddleFinger2 = 264,
	RightMiddleFinger3 = 265,
	RightMiddleFinger4 = 266,
	RightPinky = 271,
	RightPinky2 = 272,
	RightPinky3 = 273,
	RightPinky4 = 274,
	RightPointy = 259,
	RightPointy2 = 260,
	RightPointy3 = 261,
	RightPointy4 = 262,
	RightRingFinger = 267,
	RightRingFinger2 = 268,
	RightRingFinger3 = 269,
	RightRingFinger4 = 270,
	RightShoulder = 1282,
	RightShoulderBlade = 1295,
	RightShoulderTop = 1281,
	RightSleeve = 1286,
	RightThumb = 256,
	RightThumb2 = 257,
	RightThumb3 = 258,
	RightToes = 18,
	RightUpperArmShield = 1283,
	RightUpperLeg = 1297,
	Unknown1 = 1287,
	Unknown2 = 1294,
	Unknown3 = 1792,
	Unknown4 = 1793,
	Unknown5 = 1794,
	Unknown6 = 1795
};


class PlayerManager
{
public:
	char pad_0004[4]; //0x0004
	uint32_t N0000000D; //0x0008
	char pad_000C[12]; //0x000C
	uint32_t N00000011; //0x0018
	uint32_t N00000012; //0x001C
	uint32_t N00000013; //0x0020
	char pad_0024[16]; //0x0024
	uint32_t N00000018; //0x0034
	uint32_t N00000019; //0x0038
	uint32_t N0000001A; //0x003C
	char pad_0040[16]; //0x0040
	uint32_t N0000002A; //0x0050
	uint32_t N0000002B; //0x0054
	uint32_t N0000002C; //0x0058
	char pad_005C[16]; //0x005C
	uint32_t N00000031; //0x006C
	uint32_t N00000032; //0x0070
	uint32_t N00000033; //0x0074
	char pad_0078[16]; //0x0078
	uint32_t N00000038; //0x0088
	uint32_t N00000039; //0x008C
	uint32_t N0000003A; //0x0090
	char pad_0094[28]; //0x0094
	Vector4 Position; //0x00B0
	char pad_00C0[12]; //0x00C0
	uint32_t N00000046; //0x00CC
	float N00000047; //0x00D0
	int32_t HP_Increase; //0x00D4
	uint32_t N00000049; //0x00D8
	uint32_t Cash; //0x00DC
	uint32_t MainWeaponIndex; //0x00E0
	uint32_t CustomWeaponIndex; //0x00E4
	uint32_t EquippedSubIndex; //0x00E8
	uint32_t EquippedRecoveryIndex; //0x00EC
	uint32_t N00000050; //0x00F0
	uint32_t N00000051; //0x00F4
	char pad_00F8[72]; //0x00F8

	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
	virtual void Function9();
	virtual void Function10();
	virtual void Function11();
	virtual void Function12();
	virtual void Function13();
	virtual void Function14();
	virtual void Function15();
	virtual void Function16();
	virtual void Function17();
	virtual void Function18();
	virtual void Function19();
	virtual void Function20();
	virtual void SetMainWeaponIndex();
	virtual void GetMainWeaponIndex();
	virtual void Function23();
	virtual void SetCustomWeaponIndex();
	virtual void SetEquippedSubIndex();
	virtual void GetCustomWeaponIndex();
	virtual void GetEquippedSubIndex();
	virtual void Function28();
	virtual void SetRecoveryIndex();
	virtual void GetRecoveryIndex();
	virtual void Function31();
	virtual void Function32();
	virtual void Function33();
	virtual void CanHPIncrease();
	virtual void CanHPDecrease();
	virtual void Function36();
	virtual void Function37();
	virtual void GetHPIncrease();
	virtual void Function39();
	virtual void Function40();
	virtual void Function41();
	virtual void GetCash();
	virtual void Function43();
	virtual void Function44();
}; //Size: 0x0140


class N00000581
{
public:
	char pad_0000[8]; //0x0000
	float N00000584; //0x0008
	float N00000585; //0x000C
	float N00000586; //0x0010
	float N00000587; //0x0014
	float N00000588; //0x0018
	float N00000589; //0x001C
	float N0000058A; //0x0020
	float N0000058B; //0x0024
	float N0000058C; //0x0028
	float N0000058D; //0x002C
	float N0000058E; //0x0030
	float N0000058F; //0x0034
	float N00000590; //0x0038
	float N00000591; //0x003C
	float N00000592; //0x0040
}; //Size: 0x0044

class cMesh
{
public:
	char pad_0000[16]; //0x0000
	float ArmorGlowR; //0x0010
	float ArmorGlowG; //0x0014
	float ArmowGlowB; //0x0018
	float ArmowAlpha; //0x001C
	float N000005AC; //0x0020
	float N000005AD; //0x0024
	float N000005AE; //0x0028
	float N000005AF; //0x002C
	char pad_0030[16]; //0x0030
	float N000005B4; //0x0040
	float N000005BD; //0x0044
	float N000005BE; //0x0048
	float N000005BF; //0x004C
	float ArmorGlowDiscoR; //0x0050
	float ArmorGlowDiscoG; //0x0054
	float ArmorGlowDiscoB; //0x0058
	float ArmorGlowDiscoAlpha; //0x005C
	char pad_0060[16]; //0x0060
}; //Size: 0x0070


class cPart
{
public:
	char pad_0004[88]; //0x0004
	float N000006CD; //0x005C
	char pad_0060[16]; //0x0060
	float X; //0x0070
	float Y; //0x0074
	float Z; //0x0078
	char pad_007C[4]; //0x007C
	float N000006BC; //0x0080
	float N000004D0; //0x0084
	float N000004D1; //0x0088
	char pad_008C[20]; //0x008C
	uint16_t PartIndex; //0x00A0
	uint16_t Freeze; //0x00A2
	char pad_00A4[4]; //0x00A4
	class PTenThousand* Owner; //0x00A8
	char pad_00AC[4]; //0x00AC

	virtual void Function0();
}; //Size: 0x00B0

class RaidenParts
{
public:
	class cPart* FullBody; //0x0000
	class cPart* Chest; //0x0004
	class cPart* Chest2; //0x0008
	class cPart* Chest3; //0x000C
	class cPart* Head; //0x0010
	class cPart* Head2; //0x0014
	class cPart* RightArm1; //0x0018
	class cPart* RightArm2; //0x001C
	class cPart* RightLowerArm; //0x0020
	class cPart* RightHand; //0x0024
	class cPart* LeftArm; //0x0028
	class cPart* LeftArm2; //0x002C
	class cPart* LeftLowerArm; //0x0030
	class cPart* LeftHand; //0x0034
	class cPart* RightLegScale; //0x0038
	class cPart* RightLowerLeg; //0x003C
	class cPart* RightFoot; //0x0040
	class cPart* RightToes; //0x0044
	class cPart* LeftLegSize; //0x0048
	class cPart* LeftLowerLeg; //0x004C
	class cPart* LeftFoot; //0x0050
	class cPart* LeftToes; //0x0054
	class cPart* RightThumb; //0x0058
	class cPart* MoreRightThumb; //0x005C
	class cPart* EvenMoreRightThumb; //0x0060
	class cPart* RightPointy; //0x0064
	class cPart* RightPointy2; //0x0068
	class cPart* RightPointy3; //0x006C
	class cPart* RightPointy4; //0x0070
	class cPart* RightMiddleFinger; //0x0074
	class cPart* RightMiddleFinger2; //0x0078
	class cPart* RightMiddleFinger3; //0x007C
	class cPart* RightMiddleFinger4; //0x0080
	class cPart* RightRingFinger; //0x0084
	class cPart* RightRingFinger2; //0x0088
	class cPart* RightRingFinger3; //0x008C
	class cPart* RightRingFinger4; //0x0090
	class cPart* RightPinky; //0x0094
	class cPart* RightPinky2; //0x0098
	class cPart* RightPinky3; //0x009C
	class cPart* RightPinky4; //0x00A0
	class cPart* LeftThumb; //0x00A4
	class cPart* LeftThumb2; //0x00A8
	class cPart* LeftThumb3; //0x00AC
	class cPart* LeftPointy; //0x00B0
	class cPart* LeftPointy2; //0x00B4
	class cPart* LeftPointy3; //0x00B8
	class cPart* LeftPointy4; //0x00BC
	class cPart* LeftMiddleFinger; //0x00C0
	class cPart* LeftMiddleFinger2; //0x00C4
	class cPart* LeftMiddleFinger3; //0x00C8
	class cPart* LeftMiddleFinger4; //0x00CC
	class cPart* LeftRingFinger; //0x00D0
	class cPart* LeftRingFinger2; //0x00D4
	class cPart* LeftRingFinger3; //0x00D8
	class cPart* LeftRingFinger4; //0x00DC
	class cPart* LeftPinky; //0x00E0
	class cPart* LeftPinky2; //0x00E4
	class cPart* LeftPinky3; //0x00E8
	class cPart* LeftPinky4; //0x00EC
	class cPart* Neck; //0x00F0
	class cPart* RightShoulderTop; //0x00F4
	class cPart* RightShoulder; //0x00F8
	class cPart* RightUpperArmShield; //0x00FC
	class cPart* RightElbow; //0x0100
	class cPart* RightLowerArmShield; //0x0104
	class cPart* RightSleeve; //0x0108
	class cPart* N000005C1; //0x010C
	class cPart* LeftShoulderTop; //0x0110
	class cPart* LeftShoulder; //0x0114
	class cPart* LeftUpperArmShield; //0x0118
	class cPart* LeftElbow; //0x011C
	class cPart* LeftLowerArmShield; //0x0120
	class cPart* LeftSleeve; //0x0124
	class cPart* N000005C8; //0x0128
	class cPart* RightShoulderBlade; //0x012C
	class cPart* LeftShoulderBlade; //0x0130
	class cPart* RightUpperLeg; //0x0134
	class cPart* RightKneepad; //0x0138
	class cPart* LeftUpperLeg; //0x013C
	class cPart* LeftKneepad; //0x0140
	class cPart* RightButt; //0x0144
	class cPart* LeftButt; //0x0148
	class cPart* N000005D1; //0x014C
	class cPart* N000005D2; //0x0150
	class cPart* N000005D3; //0x0154
	class cPart* N000005D4; //0x0158
	class cPart* SwordHolsterOnBack; //0x015C
	class cPart* N000005D6; //0x0160
	class cPart* N000005D7; //0x0164
	class cPart* N000005D8; //0x0168
	class cPart* N000005D9; //0x016C
	class cPart* N000005DA; //0x0170
	class cPart* N000005DB; //0x0174
	class cPart* N000005DC; //0x0178
}; //Size: 0x017C


class N00000F7B
{
public:
	char pad_0004[60]; //0x0004

	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
	virtual void Function9();
}; //Size: 0x0040

class N00001A91
{
public:
	char pad_0000[4]; //0x0000
	class cPart* Body; //0x0004
	class cPart* UpperBody; //0x0008
	class cPart* UpperBodyThickness; //0x000C
	class cPart* UpperBodyWidth; //0x0010
	class cPart* RightArm; //0x0014
	class cPart* RightArmWidth; //0x0018
}; //Size: 0x001C

enum EPTenThousandVTable {
	TakeDamage = 195
};

class PTenThousand
{
public:
	char pad_0004[12]; //0x0004
	float N000002B2; //0x0010
	char pad_0014[4]; //0x0014
	float N000002B4; //0x0018
	char pad_001C[8]; //0x001C
	float N000002B7; //0x0024
	char pad_0028[8]; //0x0028
	float N000002BA; //0x0030
	char pad_0034[4]; //0x0034
	float N000002BC; //0x0038
	char pad_003C[4]; //0x003C
	Vector4 N000002BE; //0x0040
	Vector4 PlayerPosition; //0x0050
	char pad_0060[16]; //0x0060
	float ModelWidth; //0x0070
	float ModelHeight; //0x0074
	float ModelLength; //0x0078
	char pad_007C[52]; //0x007C
	float N000002D5; //0x00B0
	char pad_00B4[16]; //0x00B4
	float N000002DA; //0x00C4
	char pad_00C8[16]; //0x00C8
	float N000002DF; //0x00D8
	char pad_00DC[16]; //0x00DC
	float N000002E4; //0x00EC
	float N000002E5; //0x00F0
	char pad_00F4[16]; //0x00F4
	float N000002EA; //0x0104
	char pad_0108[16]; //0x0108
	float N000002EF; //0x0118
	char pad_011C[16]; //0x011C
	float N000002F4; //0x012C
	char pad_0130[12]; //0x0130
	float N000002F8; //0x013C
	char pad_0140[236]; //0x0140
	float N00000303; //0x022C
	char pad_0230[12]; //0x0230
	uint16_t N00000307; //0x023C
	uint16_t N00000536; //0x023E
	char pad_0240[192]; //0x0240
	class N00000581* N00000338; //0x0300
	char pad_0304[28]; //0x0304
	class cMesh* ArmorMeshes; //0x0320
	char pad_0324[48]; //0x0324
	class RaidenParts* RaidenPartCollection; //0x0354
	char pad_0358[1020]; //0x0358
	class N00000F7B* BattleParamter; //0x0754
	char pad_0758[280]; //0x0758
	uint32_t CurrentHP; //0x0870
	uint32_t BaseHP; //0x0874
	char pad_0878[6944]; //0x0878
	class N00001A91* SomeArray; //0x2398
	uint32_t SomeArrayCount; //0x239C
	char pad_23A0[20]; //0x23A0

	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
	virtual void Function9();
	virtual void Function10();
	virtual void Function11();
	virtual void Function12();
	virtual void Function13();
	virtual void Function14();
	virtual void Function15();
	virtual void Function16();
	virtual void Function17();
	virtual void Function18();
	virtual void Function19();
	virtual void Function20();
	virtual void Function21();
	virtual void Function22();
	virtual void Function23();
	virtual void Function24();
	virtual void Function25();
	virtual void Function26();
	virtual void Function27();
	virtual void Function28();
	virtual void Function29();
	virtual void Function30();
	virtual void Function31();
	virtual void Function32();
	virtual void Function33();
	virtual void Function34();
	virtual void Function35();
	virtual void Function36();
	virtual void Function37();
	virtual void Function38();
	virtual void Function39();
	virtual void Function40();
	virtual void Function41();
	virtual void Function42();
	virtual void Function43();
	virtual void Function44();
	virtual void Function45();
	virtual void Function46();
	virtual void Function47();
	virtual void Function48();
	virtual void Function49();
	virtual void Function50();
	virtual void Function51();
	virtual void Function52();
	virtual void Function53();
	virtual void Function54();
	virtual void Function55();
	virtual void Function56();
	virtual void Function57();
	virtual void Function58();
	virtual void Function59();
	virtual void Function60();
	virtual void Function61();
	virtual void Function62();
	virtual void Function63();
	virtual void Function64();
	virtual void Function65();
	virtual void Function66();
	virtual void Function67();
	virtual void Function68();
	virtual void Function69();
	virtual void Function70();
	virtual void Function71();
	virtual void Function72();
	virtual void Function73();
	virtual void Function74();
	virtual void Function75();
	virtual void Function76();
	virtual void Function77();
	virtual void Function78();
	virtual void Function79();
	virtual void Function80();
	virtual void Function81();
	virtual void Function82();
	virtual void Function83();
	virtual void Function84();
	virtual void Function85();
	virtual void Function86();
	virtual void Function87();
	virtual void Function88();
	virtual void Function89();
	virtual void Function90();
	virtual void Function91();
	virtual void Function92();
	virtual void Function93();
	virtual void Function94();
	virtual void Function95();
	virtual void Function96();
	virtual void Function97();
	virtual void Function98();
	virtual void Function99();
	virtual void Function100();
	virtual void Function101();
	virtual void Function102();
	virtual void Function103();
	virtual void Function104();
	virtual void Function105();
	virtual void Function106();
	virtual void Function107();
	virtual void Function108();
	virtual void Function109();
	virtual void Function110();
	virtual void Function111();
	virtual void Function112();
	virtual void Function113();
	virtual void Function114();
	virtual void Function115();
	virtual void Function116();
	virtual void Function117();
	virtual void Function118();
	virtual void Function119();
	virtual void Function120();
	virtual void Function121();
	virtual void Function122();
	virtual void Function123();
	virtual void Function124();
	virtual void Function125();
	virtual void Function126();
	virtual void Function127();
	virtual void Function128();
	virtual void Function129();
	virtual void Function130();
	virtual void Function131();
	virtual void Function132();
	virtual void Function133();
	virtual void Function134();
	virtual void Function135();
	virtual void Function136();
	virtual void Function137();
	virtual void Function138();
	virtual void Function139();
	virtual void Function140();
	virtual void Function141();
	virtual void Function142();
	virtual void Function143();
	virtual void Function144();
	virtual void Function145();
	virtual void Function146();
	virtual void Function147();
	virtual void Function148();
	virtual void Function149();
	virtual void Function150();
	virtual void Function151();
	virtual void Function152();
	virtual void Function153();
	virtual void Function154();
	virtual void Function155();
	virtual void Function156();
	virtual void Function157();
	virtual void Function158();
	virtual void Function159();
	virtual void Function160();
	virtual void Function161();
	virtual void Function162();
	virtual void Function163();
	virtual void Function164();
	virtual void Function165();
	virtual void Function166();
	virtual void Function167();
	virtual void Function168();
	virtual void Function169();
	virtual void Function170();
	virtual void Function171();
	virtual void Function172();
	virtual void Function173();
	virtual void Function174();
	virtual void Function175();
	virtual void Function176();
	virtual void Function177();
	virtual void Function178();
	virtual void Function179();
	virtual void Function180();
	virtual void Function181();
	virtual void Function182();
	virtual void Function183();
	virtual void Function184();
	virtual void Function185();
	virtual void Function186();
	virtual void Function187();
	virtual void Function188();
	virtual void Function189();
	virtual void Function190();
	virtual void Function191();
	virtual void Function192();
	virtual void Function193();
	virtual void Function194();
	virtual void TakeDamage();
	virtual void Function196();
	virtual void Function197();
	virtual void Function198();
	virtual void Function199();
	virtual void Function200();
	virtual void Function201();
	virtual void Function202();
	virtual void Function203();
	virtual void Function204();
	virtual void Function205();
	virtual void Function206();
	virtual void Function207();
	virtual void Function208();
	virtual void Function209();
	virtual void Function210();
	virtual void Function211();
	virtual void Function212();
	virtual void Function213();
	virtual void Function214();
	virtual void Function215();
	virtual void Function216();
	virtual void Function217();
	virtual void Function218();
	virtual void Function219();
	virtual void Function220();
	virtual void Function221();
	virtual void Function222();
	virtual void Function223();
	virtual void Function224();
	virtual void Function225();
	virtual void Function226();
	virtual void Function227();
	virtual void Function228();
	virtual void Function229();
	virtual void Function230();
	virtual void Function231();
	virtual void Function232();
	virtual void Function233();
	virtual void Function234();
	virtual void Function235();
	virtual void Function236();
	virtual void Function237();
	virtual void Function238();
	virtual void Function239();
	virtual void Function240();
	virtual void Function241();
	virtual void Function242();
	virtual void Function243();
	virtual void Function244();
	virtual void Function245();
	virtual void Function246();
	virtual void Function247();
	virtual void Function248();
	virtual void Function249();
	virtual void Function250();
	virtual void Function251();
	virtual void Function252();
	virtual void Function253();
}; //Size: 0x23B4


class N00000E41
{
public:
	char pad_0000[4]; //0x0000
}; //Size: 0x0004

class N00000DDD
{
public:
	class N00000E41* N00000DDF; //0x0004
	uint32_t ElementCount; //0x0008
	char pad_000C[8]; //0x000C

	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
	virtual void Function9();
}; //Size: 0x0014

class N00000F1C
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044

class N0000112F
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044

class GameWorkManagerImpl
{
public:
	char pad_0004[4]; //0x0004
	class N00000F1C* N00000E82; //0x0008
	char pad_000C[320]; //0x000C
	class N0000112F* N00000F31; //0x014C
	char pad_0150[12]; //0x0150

	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
	virtual void Function9();
	virtual void Function10();
	virtual void Function11();
	virtual void Function12();
	virtual void Function13();
	virtual void Function14();
	virtual void Function15();
	virtual void Function16();
	virtual void Function17();
	virtual void Function18();
	virtual void Function19();
	virtual void Function20();
	virtual void Function21();
	virtual void Function22();
	virtual void Function23();
	virtual void Function24();
	virtual void Function25();
	virtual void Function26();
	virtual void Function27();
	virtual void Function28();
	virtual void Function29();
	virtual void Function30();
	virtual void Function31();
	virtual void Function32();
	virtual void Function33();
	virtual void Function34();
	virtual void Function35();
	virtual void Function36();
	virtual void Function37();
	virtual void Function38();
	virtual void Function39();
	virtual void Function40();
	virtual void Function41();
	virtual void Function42();
	virtual void Function43();
	virtual void Function44();
}; //Size: 0x015C

class N00001149
{
public:
	char pad_0004[60]; //0x0004

	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
}; //Size: 0x0040

class N00001165
{
public:
	char pad_0004[60]; //0x0004

	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
	virtual void Function9();
}; //Size: 0x0040





void RandomizeRaiden(PTenThousand* pTenK) {
	static std::default_random_engine e;
	static std::uniform_real_distribution<> dis(-2.5, 2.5);
	auto partCollection = pTenK->RaidenPartCollection;
	if (partCollection){
#pragma region Chest
		if (partCollection->Chest) {
			partCollection->Chest->X = dis(e);
			partCollection->Chest->Y = dis(e);
			partCollection->Chest->Z = dis(e);
			partCollection->Chest->Freeze = 8;
		}
#pragma endregion
#pragma region Chest2
		if (partCollection->Chest2) {
			partCollection->Chest2->X = dis(e);
			partCollection->Chest2->Y = dis(e);
			partCollection->Chest2->Z = dis(e);
			partCollection->Chest2->Freeze = 8;
		}
#pragma endregion
#pragma region Chest3
		if (partCollection->Chest3) {
			partCollection->Chest3->X = dis(e);
			partCollection->Chest3->Y = dis(e);
			partCollection->Chest3->Z = dis(e);
			partCollection->Chest3->Freeze = 8;
		}
#pragma endregion

#pragma region Legs
		if (partCollection->LeftLowerLeg) {
			partCollection->LeftLowerLeg->X = dis(e);
			partCollection->LeftLowerLeg->Y = dis(e);
			partCollection->LeftLowerLeg->Freeze = 8;
		}
		if (partCollection->RightLowerLeg) {
			partCollection->RightLowerLeg->X = dis(e);
			partCollection->RightLowerLeg->Y = dis(e);
			partCollection->RightLowerLeg->Freeze = 8;
		}
#pragma endregion

#pragma region Head
		if (partCollection->Head2) {
			partCollection->Head2->X = dis(e);
			partCollection->Head2->Freeze = dis(e);
		}
#pragma endregion
	}
}