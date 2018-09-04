//----- (00436940) --------------------------------------------------------
void __fastcall pfile_436940_tolower(char *c)
{
  char *i; // [esp+Ch] [ebp-4h]

  for ( i = c; *i; ++i )
    *i = tolower(*i);
}

//----- (00436980) --------------------------------------------------------
void __cdecl pfile_436980_init_savename()
{
  char c[260]; // [esp+Ch] [ebp-208h]
  char Buffer[260]; // [esp+110h] [ebp-104h]

  if ( !GetCurrentDirectoryA(0x104u, Buffer) )
    TermMsg("Unable to get current directory");
  pfile_436940_tolower(Buffer);
  if ( GetDriveTypeA(0) == 5 )
  {
    if ( !GetTempPathA(0x104u, c) )
      TermMsg("Unable to locate temp directory");
    pfile_436940_tolower(c);
    if ( !strcmp(c, Buffer) )
      TermMsg("Temp directory is on non-writable drive");
    strcpy(FileName, c);
  }
  else
  {
    strcpy(FileName, Buffer);
  }
  strcat(FileName, "\\diablo.sv");
}

//----- (00436A75) --------------------------------------------------------
void __fastcall pfile_436A75_reverse_name(int pnum)
{
  unsigned int v2; // [esp+10h] [ebp-4h]

  if ( pnum == myplr )
  {
    v2 = pfile_get_save_num_from_name(plr[pnum]._pName);
    if ( v2 >= 0x14 )
      assertion_failed(123, "C:\\Diablo\\Direct\\pfile.cpp");
    _strrev(hero_names[v2]);
    _strrev(name);
  }
  _strrev(plr[pnum]._pName);
}

//----- (00436B28) --------------------------------------------------------
int __fastcall pfile_get_save_num_from_name(char *name)
{
  const char *v2; // [esp+Ch] [ebp-8h]
  unsigned int i; // [esp+10h] [ebp-4h]

  v2 = name;
  for ( i = 0; i < 0x14 && _strcmpi(hero_names[i], v2); ++i )
    ;
  return i;
}

//----- (00436B84) --------------------------------------------------------
void __cdecl pfile_write_hero()
{
  int v0; // eax
  PkPlayerStruct pPack; // [esp+Ch] [ebp-3D8h]

  if ( myplr < 0 || myplr >= 4 )
    assertion_failed(285, "C:\\Diablo\\Direct\\pfile.cpp");
  PackPlayer(&pPack, myplr);
  v0 = pfile_get_save_num_from_name(pPack.pName);
  pfile_encode_hero(v0, &pPack);
}

//----- (00436BF3) --------------------------------------------------------
void __fastcall pfile_encode_hero(unsigned int a1, void *a2)
{
  int dwBytes; // ST24_4
  char *v3; // edx
  char *v4; // ecx
  const void *v5; // [esp+Ch] [ebp-128h]
  char Buffer[16]; // [esp+18h] [ebp-11Ch]
  void *pbSrcDst; // [esp+28h] [ebp-10Ch]
  char a2a; // [esp+2Ch] [ebp-108h]
  int nSize; // [esp+130h] [ebp-4h]

  v5 = a2;
  if ( a1 >= 0x14 )
    assertion_failed(258, "C:\\Diablo\\Direct\\pfile.cpp");
  pfile_436D33(a1, &a2a);
  strcpy(Buffer, "xgr1");
  *(_DWORD *)&Buffer[5] = 0;
  *(_DWORD *)&Buffer[9] = 0;
  *(_WORD *)&Buffer[13] = 0;
  Buffer[15] = 0;
  nSize = 16;
  GetComputerNameA(Buffer, (LPDWORD)&nSize);
  dwBytes = codec_get_encoded_len(982);
  pbSrcDst = DiabloAllocPtr(dwBytes, 269, "C:\\Diablo\\Direct\\pfile.cpp");
  memcpy(pbSrcDst, v5, 0x3D6u);
  codec_encode(pbSrcDst, 982, dwBytes, Buffer);
  mpqapi_write_file(v4, v3, (int)FileName);
  empty_fn_4();
  mem_free_dbg(pbSrcDst, 278, "C:\\Diablo\\Direct\\pfile.cpp");
}

//----- (00436D33) --------------------------------------------------------
void __fastcall pfile_436D33(int n, char *buf)
{
  unsigned __int8 v2; // [esp+14h] [ebp-4h]

  if ( (signed int)gbMaxPlayers <= 1 )
    v2 = 1;
  else
    v2 = 4;
  sprintf(buf, "h%02d%02d", n, v2);
}

//----- (00436D84) --------------------------------------------------------
void __fastcall game_2_ui_player(PlayerStruct *a1, _uiheroinfo *a2)
{
  strncpy(a2->name, a1->_pName, 15u);
  a2->name[15] = 0;
  a2->level = a1->_pLevel;
  a2->heroclass = game_2_ui_class(a1);
  a2->herorank = a1->_pStrength;
  a2->magic = a1->_pMagic;
  a2->dexterity = a1->_pDexterity;
  a2->vitality = a1->_pVitality;
  a2->gold = a1->_pGold;
}

//----- (00436E29) --------------------------------------------------------
char __fastcall game_2_ui_class(PlayerStruct *a1)
{
  if ( !a1->_pClass )
    return 0;
  if ( a1->_pClass == 1 )
    return 1;
  return 2;
}

//----- (00436E74) --------------------------------------------------------
bool __stdcall pfile_ui_set_hero_infos(void (__stdcall *ui_add_hero_info)(_uiheroinfo *))
{
  int v1; // edx
  int *v2; // ecx
  _uiheroinfo a2; // [esp+Ch] [ebp-408h]
  BOOL v5; // [esp+2Ch] [ebp-3E8h]
  PkPlayerStruct pPack; // [esp+30h] [ebp-3E4h]
  char a3; // [esp+408h] [ebp-Ch]
  void *v8; // [esp+40Ch] [ebp-8h]
  unsigned int a1a; // [esp+410h] [ebp-4h]

  memset(hero_names, 0, 640u);
  v8 = pfile_open_save_archive(v2, v1);
  if ( !v8 )
    return 1;
  a3 = 0;
  for ( a1a = 0; a1a < 20; ++a1a )
  {
    v5 = pfile_read_hero(a1a, &pPack, &a3);
    if ( a3 )
      break;
    if ( v5 )
    {
      strcpy(hero_names[a1a], pPack.pName);
      UnPackPlayer(&pPack, 0);
      game_2_ui_player(plr, &a2);
      ui_add_hero_info(&a2);
    }
  }
  SFileCloseArchive(v8);
  if ( a3 )
    DeleteFileA(FileName);
  else
    empty_fn_4();
  return 1;
}

//----- (00436F91) --------------------------------------------------------
void *__fastcall pfile_open_save_archive(int *unused, int save_num)
{
  void *v3; // [esp+Ch] [ebp-4h]

  if ( !empty_fn_2() )
    return 0;
  if ( SFileOpenArchive(FileName, 28672, 0, &v3) )
    return v3;
  return 0;
}

//----- (00436FE4) --------------------------------------------------------
BOOL __fastcall pfile_read_hero(unsigned int a1, PkPlayerStruct *pPack, char *a3)
{
  PkPlayerStruct *v4; // [esp+Ch] [ebp-134h]
  int v5; // [esp+14h] [ebp-12Ch]
  char Buffer[16]; // [esp+18h] [ebp-128h]
  BOOL v7; // [esp+28h] [ebp-118h]
  int dwBytes; // [esp+2Ch] [ebp-114h]
  char v9[260]; // [esp+30h] [ebp-110h]
  int v10; // [esp+134h] [ebp-Ch]
  void *pbSrcDst; // [esp+138h] [ebp-8h]
  int nSize; // [esp+13Ch] [ebp-4h]

  v4 = pPack;
  if ( a1 >= 0x14 )
    assertion_failed(185, "C:\\Diablo\\Direct\\pfile.cpp");
  if ( !pPack )
    assertion_failed(186, "C:\\Diablo\\Direct\\pfile.cpp");
  if ( !a3 )
    assertion_failed(187, "C:\\Diablo\\Direct\\pfile.cpp");
  pfile_436D33(a1, v9);
  if ( !SFileOpenFile(v9, &v10) )
    return 0;
  *a3 = 0;
  pbSrcDst = 0;
  v7 = 0;
  strcpy(Buffer, "xgr1");
  *(_DWORD *)&Buffer[5] = 0;
  *(_DWORD *)&Buffer[9] = 0;
  *(_WORD *)&Buffer[13] = 0;
  Buffer[15] = 0;
  nSize = 16;
  GetComputerNameA(Buffer, (LPDWORD)&nSize);
  dwBytes = SFileGetFileSize(v10, 0);
  pbSrcDst = DiabloAllocPtr(dwBytes, 208, "C:\\Diablo\\Direct\\pfile.cpp");
  if ( SFileReadFile(v10, (int)pbSrcDst, dwBytes, (int)&v5, 0) )
  {
    if ( v5 != dwBytes )
      assertion_failed(212, "C:\\Diablo\\Direct\\pfile.cpp");
    v5 = codec_decode(pbSrcDst, dwBytes, Buffer);
    if ( v5 == 982 )
    {
      memcpy(v4, pbSrcDst, 0x3D6u);
      v7 = 1;
    }
    else
    {
      *a3 = 1;
    }
  }
  if ( pbSrcDst )
    mem_free_dbg(pbSrcDst, 229, "C:\\Diablo\\Direct\\pfile.cpp");
  SFileCloseFile(v10);
  return v7;
}

//----- (00437209) --------------------------------------------------------
bool __stdcall pfile_ui_set_class_stats(int player_class_nr, _uidefaultstats *class_stats)
{
  char v2; // al

  v2 = pfile_get_player_class(player_class_nr);
  class_stats->strength = StrengthTbl[v2];
  class_stats->magic = MagicTbl[v2];
  class_stats->dexterity = DexterityTbl[v2];
  class_stats->vitality = VitalityTbl[v2];
  return 1;
}

//----- (00437270) --------------------------------------------------------
char __fastcall pfile_get_player_class(int player_class_nr)
{
  if ( !player_class_nr )
    return 0;
  if ( player_class_nr == 1 )
    return 1;
  return 2;
}

//----- (004372AA) --------------------------------------------------------
bool __stdcall pfile_ui_save_create(_uiheroinfo *heroinfo)
{
  bool (__stdcall *v1)(int, char *); // ecx
  char v3; // al
  PkPlayerStruct pPack; // [esp+Ch] [ebp-3DCh]
  unsigned int i; // [esp+3E4h] [ebp-4h]

  if ( !heroinfo->name[0] )
    assertion_failed(384, "C:\\Diablo\\Direct\\pfile.cpp");
  i = pfile_get_save_num_from_name(heroinfo->name);
  if ( i == 20 )
  {
    for ( i = 0; i < 0x14 && hero_names[i][0]; ++i )
      ;
  }
  if ( i == 20 )
    return 0;
  pfile_4C922C_count = i;
  mpqapi_remove_hash_entries(v1);
  pfile_4C922C_count = 20;
  strncpy(hero_names[i], heroinfo->name, 0x20u);
  hero_names[i][31] = 0;
  v3 = pfile_get_player_class(heroinfo->heroclass);
  CreatePlayer(0, v3);
  strncpy(plr[0]._pName, heroinfo->name, 0x20u);
  plr[0]._pName[31] = 0;
  PackPlayer(&pPack, 0);
  pfile_encode_hero(i, &pPack);
  game_2_ui_player(plr, heroinfo);
  return 1;
}

//----- (004373F7) --------------------------------------------------------
bool __stdcall pfile_get_file_name(int lvl, char *dst)
{
  char *v3; // [esp+Ch] [ebp-4h]

  if ( (signed int)gbMaxPlayers <= 1 )
  {
    if ( (unsigned int)lvl >= 0x11 )
    {
      if ( (unsigned int)lvl >= 0x22 )
      {
        if ( lvl == 34 )
        {
          v3 = "g%02d";
        }
        else
        {
          if ( lvl != 35 )
            return 0;
          v3 = "h%02d%02d";
          lvl = 1;
        }
      }
      else
      {
        lvl -= 17;
        v3 = "g%02dls%02d";
      }
    }
    else
    {
      v3 = "g%02dlv%02d";
    }
  }
  else
  {
    if ( lvl )
      return 0;
    lvl = 4;
    v3 = "h%02d%02d";
  }
  if ( (unsigned int)pfile_4C922C_count >= 0x14 )
    assertion_failed(164, "C:\\Diablo\\Direct\\pfile.cpp");
  sprintf(dst, v3, pfile_4C922C_count, lvl);
  return 1;
}

//----- (004374EA) --------------------------------------------------------
bool __stdcall pfile_delete_save(_uiheroinfo *hero_info)
{
  bool (__stdcall *v1)(int, char *); // ecx

  pfile_4C922C_count = pfile_get_save_num_from_name(hero_info->name);
  if ( (unsigned int)pfile_4C922C_count < 0x14 )
  {
    hero_names[pfile_4C922C_count][0] = 0;
    mpqapi_remove_hash_entries(v1);
    pfile_4C922C_count = 20;
    empty_fn_4();
  }
  return 1;
}

//----- (0043755A) --------------------------------------------------------
void __cdecl pfile_read_player_from_save()
{
  int v0; // edx
  int *v1; // ecx
  char a3; // [esp+Ch] [ebp-3E4h]
  void *v3; // [esp+10h] [ebp-3E0h]
  PkPlayerStruct a2; // [esp+14h] [ebp-3DCh]
  int hsArchive; // [esp+3ECh] [ebp-4h]

  if ( myplr < 0 || myplr >= 4 )
    assertion_failed(442, "C:\\Diablo\\Direct\\pfile.cpp");
  hsArchive = pfile_get_save_num_from_name(name);
  if ( hsArchive == 20 )
    assertion_failed(444, "C:\\Diablo\\Direct\\pfile.cpp");
  v3 = pfile_open_save_archive(v1, v0);
  if ( !v3 )
    TermMsg("Unable to open archive");
  if ( !pfile_read_hero(hsArchive, &a2, &a3) )
    TermMsg("Unable to load character");
  UnPackPlayer(&a2, myplr);
  pfile_archive_contains_game(hsArchive);
  SFileCloseArchive(v3);
}

//----- (0043763D) --------------------------------------------------------
bool __fastcall pfile_archive_contains_game(int hsArchive)
{
  int v1; // eax
  char v3[260]; // [esp+10h] [ebp-108h]
  int v4; // [esp+114h] [ebp-4h]

  if ( (unsigned int)hsArchive >= 0x14 )
    assertion_failed(238, "C:\\Diablo\\Direct\\pfile.cpp");
  LOBYTE(v1) = gbMaxPlayers;
  if ( gbMaxPlayers == 1 )
  {
    sprintf(v3, "g%02d", hsArchive);
    v1 = SFileOpenFile(v3, &v4);
    dword_6180EC = v1;
    if ( v1 )
      LOBYTE(v1) = SFileCloseFile(v4);
  }
  else
  {
    dword_6180EC = 0;
  }
  return v1;
}

//----- (004376DF) --------------------------------------------------------
void __fastcall pfile_unused_sub_4376DF(char *a1)
{
  unsigned int v2; // [esp+10h] [ebp-4h]

  v2 = pfile_get_save_num_from_name(name);
  if ( v2 >= 0x14 )
    assertion_failed(462, "C:\\Diablo\\Direct\\pfile.cpp");
  if ( setlevel )
    sprintf(a1, "g%02dls%02d", v2, (unsigned __int8)setlvlnum);
  else
    sprintf(a1, "g%02dlv%02d", v2, currlevel);
  if ( gbMaxPlayers != 1 )
    assertion_failed(465, "C:\\Diablo\\Direct\\pfile.cpp");
}

//----- (00437792) --------------------------------------------------------
void __fastcall pfile_unused_sub_437792(char *a1)
{
  unsigned int v2; // [esp+10h] [ebp-4h]

  v2 = pfile_get_save_num_from_name(name);
  if ( v2 >= 0x14 )
    assertion_failed(473, "C:\\Diablo\\Direct\\pfile.cpp");
  sprintf(a1, "g%02d", v2);
  if ( gbMaxPlayers != 1 )
    assertion_failed(475, "C:\\Diablo\\Direct\\pfile.cpp");
}

//----- (0043780C) --------------------------------------------------------
void __fastcall pfile_unused_sub_43780C(int a1, void *a2, int size, int size_64)
{
  char *v4; // edx
  char *v5; // ecx
  void *pbSrcDst; // [esp+Ch] [ebp-1Ch]
  CHAR Buffer[16]; // [esp+14h] [ebp-14h]
  DWORD nSize; // [esp+24h] [ebp-4h]

  pbSrcDst = a2;
  if ( !a1 )
    assertion_failed(482, "C:\\Diablo\\Direct\\pfile.cpp");
  if ( !a2 )
    assertion_failed(483, "C:\\Diablo\\Direct\\pfile.cpp");
  if ( !size )
    assertion_failed(484, "C:\\Diablo\\Direct\\pfile.cpp");
  if ( gbMaxPlayers != 1 )
    assertion_failed(485, "C:\\Diablo\\Direct\\pfile.cpp");
  strcpy(Buffer, "xgr1");
  *(_DWORD *)&Buffer[5] = 0;
  *(_DWORD *)&Buffer[9] = 0;
  *(_WORD *)&Buffer[13] = 0;
  Buffer[15] = 0;
  nSize = 16;
  GetComputerNameA(Buffer, &nSize);
  codec_encode(pbSrcDst, size, size_64, Buffer);
  mpqapi_write_file(v5, v4, (int)FileName);
  empty_fn_4();
}

//----- (00437919) --------------------------------------------------------
void *__fastcall pfile_unused_sub_437919(int *a1, int a2)
{
  int *v3; // [esp+Ch] [ebp-2Ch]
  int *v4; // [esp+10h] [ebp-28h]
  int v5; // [esp+14h] [ebp-24h]
  CHAR Buffer[16]; // [esp+18h] [ebp-20h]
  void *v7; // [esp+28h] [ebp-10h]
  void *pbSrcDst; // [esp+2Ch] [ebp-Ch]
  int v9; // [esp+30h] [ebp-8h]
  DWORD nSize; // [esp+34h] [ebp-4h]

  v3 = (int *)a2;
  v4 = a1;
  if ( !a1 )
    assertion_failed(501, "C:\\Diablo\\Direct\\pfile.cpp");
  if ( !a2 )
    assertion_failed(502, "C:\\Diablo\\Direct\\pfile.cpp");
  if ( gbMaxPlayers != 1 )
    assertion_failed(503, "C:\\Diablo\\Direct\\pfile.cpp");
  v7 = pfile_open_save_archive(a1, a2);
  if ( !v7 )
    TermMsg("Unable to open save file archive");
  if ( !SFileOpenFile(v4, &v9) )
    TermMsg("Unable to open save file");
  *v3 = SFileGetFileSize(v9, 0);
  if ( !*v3 )
    TermMsg("Invalid save file");
  pbSrcDst = DiabloAllocPtr(*v3, 516, "C:\\Diablo\\Direct\\pfile.cpp");
  if ( !SFileReadFile(v9, (int)pbSrcDst, *v3, (int)&v5, 0) )
    TermMsg("Unable to read save file");
  if ( *v3 != v5 )
    assertion_failed(521, "C:\\Diablo\\Direct\\pfile.cpp");
  SFileCloseFile(v9);
  SFileCloseArchive(v7);
  strcpy(Buffer, "xgr1");
  *(_DWORD *)&Buffer[5] = 0;
  *(_DWORD *)&Buffer[9] = 0;
  *(_WORD *)&Buffer[13] = 0;
  Buffer[15] = 0;
  nSize = 16;
  GetComputerNameA(Buffer, &nSize);
  *v3 = codec_decode(pbSrcDst, *v3, Buffer);
  if ( !*v3 )
    TermMsg("Invalid save file");
  return pbSrcDst;
}

//----- (00437AE8) --------------------------------------------------------
void __fastcall pfile_update(bool force_save)
{
  BOOL v1; // [esp+Ch] [ebp-8h]
  DWORD v2; // [esp+10h] [ebp-4h]

  v1 = force_save;
  if ( gbMaxPlayers != 1 )
  {
    v2 = GetTickCount();
    if ( v1 || (signed int)(v2 - dword_4C9230) > 15000 )
    {
      dword_4C9230 = v2;
      pfile_write_hero();
    }
  }
}
