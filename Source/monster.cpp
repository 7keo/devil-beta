//----- (00401000) --------------------------------------------------------
void __fastcall InitMonsterTRN(int monst, int special)
{
  char *v2; // esi
  signed int v3; // ecx
  signed int i; // [esp+14h] [ebp-10h]
  signed int j; // [esp+18h] [ebp-Ch]
  signed int v7; // [esp+1Ch] [ebp-8h]

  v2 = Monsters[monst].trans_file;
  v3 = 256;
  do
  {
    if ( *v2 == -1 )
      *v2 = 0;
    ++v2;
    --v3;
  }
  while ( v3 );
  if ( special )
    v7 = 6;
  else
    v7 = 5;
  for ( i = 0; v7 > i; ++i )
  {
    for ( j = 0; j < 8; ++j )
      Cl2ApplyTrans(
        (char *)Monsters[monst].Anims[i].Frames[j],
        Monsters[monst].trans_file,
        Monsters[monst].Anims[i].Rate);
  }
}

//----- (0040110B) --------------------------------------------------------
BOOL __fastcall monster_40110B(int a1)
{
  BOOL v3; // [esp+10h] [ebp-Ch]
  signed int j; // [esp+14h] [ebp-8h]
  signed int i; // [esp+18h] [ebp-4h]

  v3 = 0;
  for ( i = 0; currlevel >= i && !v3; ++i )
  {
    for ( j = 0; j < 16 && monster_4ED398[j] && !v3; ++j )
      v3 = monster_4ED398[i][j] - 1 > a1
        && !strcmp(monsterdata[a1].GraphicType, monsterdata[monster_4ED398[i][j] - 1].GraphicType);
  }
  return v3;
}

//----- (0040120F) --------------------------------------------------------
void __cdecl InitLevelMonsters()
{
  signed int i; // [esp+Ch] [ebp-4h]
  int j; // [esp+Ch] [ebp-4h]

  nummtypes = 0;
  monstimgtot = 0;
  MissileFileFlag = 0;
  for ( i = 0; i < 16; ++i )
    Monsters[i].mPlaceFlags = 0;
  ClrAllMonsters();
  nummonsters = 0;
  totalmonsters = 200;
  for ( j = 0; j < 200; ++j )
    monstactive[j] = j;
  uniquetrans = 0;
}

//----- (004012C4) --------------------------------------------------------
int __fastcall AddMonsterType(int type, int placeflag)
{
  int monst; // [esp+14h] [ebp-8h]
  int monsta; // [esp+14h] [ebp-8h]
  BOOL v6; // [esp+18h] [ebp-4h]

  v6 = 0;
  for ( monst = 0; monst < nummtypes && !v6; ++monst )
    v6 = Monsters[monst].mtype == type;
  monsta = monst - 1;
  if ( !v6 )
  {
    monsta = nummtypes++;
    Monsters[monsta].mtype = type;
    monstimgtot += monsterdata[type].mType;
    InitMonsterGFX(monsta);
    InitMonsterSND(monsta);
  }
  Monsters[monsta].mPlaceFlags |= placeflag;
  return monsta;
}

//----- (004013CE) --------------------------------------------------------
void __cdecl GetLevelMTypes()
{
  int v0; // eax
  int skeltypes[110]; // [esp+Ch] [ebp-4F8h]
  int v; // [esp+1C4h] [ebp-340h]
  char v3; // [esp+1C8h] [ebp-33Ch]
  int type; // [esp+1CCh] [ebp-338h]
  int v5; // [esp+1D0h] [ebp-334h]
  int mt; // [esp+1D4h] [ebp-330h]
  int v7; // [esp+1D8h] [ebp-32Ch]
  int typelist[200]; // [esp+1DCh] [ebp-328h]
  int v9; // [esp+4FCh] [ebp-8h]
  int v10; // [esp+500h] [ebp-4h]

  v3 = 3;
  AddMonsterType(108, 2);
  if ( !setlevel )
  {
    if ( quests[1]._qlevel == currlevel )
      AddMonsterType(51, 2);
    if ( quests[7]._qlevel == currlevel && quests[7]._qactive )
      AddMonsterType(UniqMonst[0].mtype, 4);
    if ( quests[10]._qlevel == currlevel && quests[10]._qactive )
      AddMonsterType(UniqMonst[2].mtype, 4);
    if ( gbMaxPlayers != 1 && quests[0]._qlevel == currlevel )
    {
      AddMonsterType(50, 4);
      v = 0;
      for ( mt = 8; mt <= 27; ++mt )
      {
        if ( IsSkel(mt) )
        {
          v5 = 15 * monsterdata[mt].mMinDLvl / 30 + 1;
          v10 = 15 * monsterdata[mt].mMaxDLvl / 30 + 1;
          if ( currlevel >= v5 && currlevel <= v10 )
          {
            if ( MonstAvailTbl[mt] & v3 )
              skeltypes[v++] = mt;
          }
        }
      }
      v0 = random(88, v);
      AddMonsterType(skeltypes[v0], 1);
    }
    v7 = 0;
    for ( mt = 0; mt < 110; ++mt )
    {
      v5 = 15 * monsterdata[mt].mMinDLvl / 30 + 1;
      v10 = 15 * monsterdata[mt].mMaxDLvl / 30 + 1;
      if ( currlevel >= v5 && currlevel <= v10 && MonstAvailTbl[mt] & v3 )
        typelist[v7++] = mt;
    }
    if ( monstdebug )
    {
      for ( mt = 0; debugmonsttypes > mt; ++mt )
        AddMonsterType(DebugMonster[mt], 1);
    }
    else
    {
      while ( v7 > 0 && nummtypes < 16 && monstimgtot < 4000 )
      {
        v9 = random(88, v7);
        type = typelist[v9];
        if ( monstimgtot + monsterdata[type].mType <= 4000 )
          AddMonsterType(type, 1);
        typelist[v9] = typelist[--v7];
      }
    }
  }
}

//----- (004017C3) --------------------------------------------------------
void __fastcall InitMonsterGFX(int monst)
{
  unsigned __int8 *v1; // eax
  signed int v3; // [esp+10h] [ebp-110h]
  char pszName; // [esp+14h] [ebp-10Ch]
  int j; // [esp+114h] [ebp-Ch]
  int i; // [esp+118h] [ebp-8h]
  DWORD *v7; // [esp+11Ch] [ebp-4h]

  v3 = Monsters[monst].mtype;
  for ( i = 0; i < 6; ++i )
  {
    if ( (animletter[i] != 's' || monsterdata[v3].has_special) && monsterdata[v3].Frames[i] > 0 )
    {
      sprintf(&pszName, monsterdata[v3].GraphicType, animletter[i]);
      if ( Monsters[monst].Anims[i].CMem )
        assertion_failed(414, "C:\\Diablo\\Direct\\MONSTER.CPP");
      v1 = LoadFileInMem(&pszName, 0, 415, "C:\\Diablo\\Direct\\MONSTER.CPP");
      Monsters[monst].Anims[i].CMem = v1;
      v7 = (DWORD *)Monsters[monst].Anims[i].CMem;
      if ( Monsters[monst].mtype != 108 || animletter[i] != 's' && animletter[i] != 'd' )
      {
        for ( j = 0; j < 8; ++j )
          Monsters[monst].Anims[i].Frames[j] = (unsigned __int8 *)v7 + v7[j];
      }
      else
      {
        for ( j = 0; j < 8; ++j )
          Monsters[monst].Anims[i].Frames[j] = (unsigned __int8 *)v7;
      }
    }
    Monsters[monst].Anims[i].Rate = monsterdata[v3].Frames[i];
    Monsters[monst].Anims[i].Delay = monsterdata[v3].Rate[i];
  }
  Monsters[monst].flags_1 = monsterdata[v3].flags;
  Monsters[monst].flags_2 = (monsterdata[v3].flags - 64) >> 1;
  Monsters[monst].mMinHP = monsterdata[v3].mMinHP;
  Monsters[monst].mMaxHP = monsterdata[v3].mMaxHP;
  Monsters[monst].has_special = monsterdata[v3].has_special;
  Monsters[monst].mAFNum = monsterdata[v3].mAFNum;
  Monsters[monst].MData = &monsterdata[v3];
  if ( monsterdata[v3].has_trans )
  {
    Monsters[monst].trans_file = (char *)LoadFileInMem(
                                           monsterdata[v3].TransFile,
                                           0,
                                           434,
                                           "C:\\Diablo\\Direct\\MONSTER.CPP");
    InitMonsterTRN(monst, monsterdata[v3].has_special);
    mem_free_dbg(Monsters[monst].trans_file, 436, "C:\\Diablo\\Direct\\MONSTER.CPP");
    Monsters[monst].trans_file = 0;
  }
  if ( v3 >= 60 && v3 <= 63 && !(MissileFileFlag & 1) )
  {
    MissileFileFlag |= 1u;
    LoadMissileGFX(0x19u);
  }
  if ( v3 >= 76 && v3 <= 79 && !(MissileFileFlag & 2) )
  {
    MissileFileFlag |= 2u;
    LoadMissileGFX(0x16u);
  }
  if ( v3 >= 100 && v3 <= 103 && !(MissileFileFlag & 4) )
  {
    MissileFileFlag |= 4u;
    LoadMissileGFX(0x17u);
    LoadMissileGFX(0x18u);
  }
  if ( v3 >= 72 && v3 <= 75 && !(MissileFileFlag & 8) )
  {
    MissileFileFlag |= 8u;
    LoadMissileGFX(0x1Au);
  }
  if ( v3 >= 46 && v3 <= 49 && !(MissileFileFlag & 0x10) )
  {
    MissileFileFlag |= 0x10u;
    LoadMissileGFX(0x20u);
    LoadMissileGFX(0x21u);
    LoadMissileGFX(0x22u);
  }
}

//----- (00401DEF) --------------------------------------------------------
void __fastcall ClearMVars(int i)
{
  monster[i]._mVar1 = 0;
  monster[i]._mVar2 = 0;
  monster[i]._mVar3 = 0;
  monster[i]._mVar4 = 0;
  monster[i]._mVar5 = 0;
  monster[i]._mVar6 = 0;
  monster[i]._mVar7 = 0;
  monster[i]._mVar8 = 0;
}

//----- (00401EC5) --------------------------------------------------------
void __fastcall InitMonster(int i, int rd, int mtype, int x, int y)
{
  CMonster *v7; // [esp+14h] [ebp-4h]

  v7 = &Monsters[mtype];
  monster[i]._mdir = rd;
  monster[i]._mx = x;
  monster[i]._my = y;
  monster[i]._mfutx = x;
  monster[i]._mfuty = y;
  monster[i]._moldx = x;
  monster[i]._moldy = y;
  monster[i]._mMTidx = mtype;
  monster[i]._mmode = 0;
  monster[i].mName = v7->MData->mName;
  monster[i].MType = v7;
  monster[i].MData = v7->MData;
  monster[i]._mAnimData = v7->Anims[0].Frames[rd];
  monster[i]._mAnimDelay = v7->Anims[0].Delay;
  monster[i]._mAnimCnt = random(88, monster[i]._mAnimDelay - 1);
  monster[i]._mAnimLen = v7->Anims[0].Rate;
  monster[i]._mAnimFrame = random(88, monster[i]._mAnimLen - 1) + 1;
  monster[i]._mmaxhp = (v7->mMinHP + random(88, v7->mMaxHP - v7->mMinHP + 1)) << 6;
  if ( gbMaxPlayers != 1 )
    monster[i]._mmaxhp = monster[i]._mmaxhp * Xmonsterhpscale[2 * gbActivePlayers - 1] / 100;
  monster[i]._mhitpoints = monster[i]._mmaxhp;
  monster[i].mAi = v7->MData->mAi;
  monster[i]._mint = v7->MData->mInt;
  monster[i]._mgoal = 1;
  monster[i]._mgoalvar1 = 0;
  monster[i]._mgoalvar2 = 0;
  monster[i]._mDelFlag = 0;
  monster[i]._uniqtype = 0;
  monster[i]._msquelch = 0;
  monster[i]._mRndSeed = GetRndSeed();
  monster[i]._mAISeed = GetRndSeed();
  monster[i].mWhoHit = 0;
  monster[i].mLevel = v7->MData->mLevel;
  monster[i].mExp = v7->MData->mExp;
  monster[i].mHit = v7->MData->mHit;
  monster[i].mMinDamage = v7->MData->mMinDamage;
  monster[i].mMaxDamage = v7->MData->mMaxDamage;
  monster[i].mHit2 = v7->MData->mHit2;
  monster[i].mMinDamage2 = v7->MData->mMinDamage2;
  monster[i].mMaxDamage2 = v7->MData->mMaxDamage2;
  monster[i].mArmorClass = v7->MData->mArmorClass;
  monster[i].mMagicRes = v7->MData->mMagicRes;
  monster[i].leader = 0;
  monster[i].leaderflag = 0;
  monster[i]._mFlags = 1;
  monster[i].mtalkmsg = 0;
  if ( (signed int)v7->mtype >= 29 && (signed int)v7->mtype <= 32 )
    monster[i]._mFlags &= 0xFFFFFFFE;
  if ( (signed int)v7->mtype >= 80 && (signed int)v7->mtype <= 83 )
  {
    monster[i]._mAnimData = v7->Anims[5].Frames[rd];
    monster[i]._mAnimFrame = 1;
    monster[i]._mFlags |= 4u;
    monster[i]._mmode = 7;
  }
}

//----- (004024FB) --------------------------------------------------------
void __cdecl ClrAllMonsters()
{
  int i; // [esp+Ch] [ebp-4h]

  for ( i = 0; i < 200; ++i )
  {
    ClearMVars(i);
    monster[i]._mgoal = 0;
    monster[i]._mmode = 0;
    monster[i]._mVar1 = 0;
    monster[i]._mVar2 = 0;
    monster[i]._mx = 0;
    monster[i]._my = 0;
    monster[i]._mfutx = 0;
    monster[i]._mfuty = 0;
    monster[i]._moldx = 0;
    monster[i]._moldy = 0;
    monster[i]._mdir = random(89, 8);
    monster[i]._mxvel = 0;
    monster[i]._myvel = 0;
    monster[i]._mAnimData = 0;
    monster[i]._mAnimDelay = 0;
    monster[i]._mAnimCnt = 0;
    monster[i]._mAnimLen = 0;
    monster[i]._mAnimFrame = 0;
    monster[i]._mFlags = 1;
    monster[i]._mDelFlag = 0;
    monster[i]._menemy = random(89, gbActivePlayers);
  }
}

//----- (00402746) --------------------------------------------------------
BOOL __fastcall MonstPlace(int xp, int yp)
{
  if ( dMonster[xp][yp] )
    return 0;
  if ( dPlayer[xp][yp] )
    return 0;
  if ( dFlags[xp][yp] & 2 )
    return 0;
  if ( dFlags[xp][yp] & 8 )
    return 0;
  return SolidLoc(xp, yp) == 0;
}

//----- (00402818) --------------------------------------------------------
void __fastcall PlaceMonster(int i, int mtype, int x, int y)
{
  int rd; // ST20_4

  dMonster[x][y] = i + 1;
  rd = random(90, 8);
  InitMonster(i, rd, mtype, x, y);
}

//----- (0040288D) --------------------------------------------------------
void __fastcall PlaceUniqueMonst(int uniqindex, int miniontype, int packsize, int num)
{
  UniqMonstStruct *UniqMonst_; // [esp+14h] [ebp-64h]
  MonsterStruct *Monst; // [esp+18h] [ebp-60h]
  int y; // [esp+1Ch] [ebp-5Ch]
  int yp; // [esp+20h] [ebp-58h]
  int xp; // [esp+24h] [ebp-54h]
  int x; // [esp+28h] [ebp-50h]
  signed int v12; // [esp+30h] [ebp-48h]
  int v13; // [esp+34h] [ebp-44h]
  char pszName[64]; // [esp+38h] [ebp-40h] MAPDST

  pszName[0] = uniqindex;
  UniqMonst_ = &UniqMonst[uniqindex];
  Monst = &monster[nummonsters];
  v13 = 0;
  while ( 1 )
  {
    x = random(91, 80) + 16;
    y = random(91, 80) + 16;
    v12 = 0;
    for ( xp = x - 3; x + 3 > xp; ++xp )
    {
      for ( yp = y - 3; y + 3 > yp; ++yp )
      {
        if ( yp >= 0 && yp < 112 && xp >= 0 && xp < 112 )
        {
          if ( MonstPlace(xp, yp) )
            ++v12;
        }
      }
    }
    if ( v12 < 9 && ++v13 < 1000 )
      continue;
    if ( MonstPlace(x, y) )
      break;
  }
  PlaceMonster(nummonsters, miniontype, x, y);
  Monst->_uniqtype = pszName[0] + 1;
  Monst->mLevel = 2 * UniqMonst_->mlevel;
  Monst->mExp *= 2;
  Monst->mName = UniqMonst_->mName;
  Monst->_mmaxhp = UniqMonst_->mmaxhp << 6;
  if ( gbMaxPlayers != 1 )
    Monst->_mmaxhp = Monst->_mmaxhp * Xmonsterhpscale[2 * gbActivePlayers - 2] / 100;
  Monst->mAi = UniqMonst_->mAi;
  Monst->_mint = UniqMonst_->mint;
  Monst->_mhitpoints = Monst->_mmaxhp;
  Monst->mMinDamage = UniqMonst_->mMinDamage;
  Monst->mMaxDamage = UniqMonst_->mMaxDamage;
  Monst->mMinDamage2 = UniqMonst_->mMinDamage;
  Monst->mMaxDamage2 = UniqMonst_->mMaxDamage;
  Monst->mMagicRes = UniqMonst_->mMagicRes;
  Monst->mtalkmsg = UniqMonst_->mtalkmsg;
  Monst->mlid = AddLight(Monst->_mx, Monst->_my, 3);
  if ( Monst->mtalkmsg )
    Monst->_mgoal = 7;
  sprintf(pszName, "Monsters\\Monsters\\%s.TRN", UniqMonst_->mMode);
  LoadFileWithMem(pszName, &pLightTbl[256 * uniquetrans + 4864]);
  Monst->_uniqtrans = uniquetrans++;
  if ( UniqMonst_->mUnqAttr & 4 )
  {
    Monst->mHit = UniqMonst_->mUnqVar1;
    Monst->mHit2 = UniqMonst_->mUnqVar1;
  }
  if ( UniqMonst_->mUnqAttr & 8 )
    Monst->mArmorClass = UniqMonst_->mUnqVar1;
  ++nummonsters;
  if ( UniqMonst_->mUnqAttr & 1 )
    PlaceGroup(packsize, num, UniqMonst_->mUnqAttr, nummonsters - 1);
}

//----- (00402C06) --------------------------------------------------------
void __cdecl PlaceQuestMonsters()
{
  int miniontype; // [esp+Ch] [ebp-Ch]
  int miniontypea; // [esp+Ch] [ebp-Ch]
  BOOL v2; // [esp+10h] [ebp-8h]
  int uniqindex; // [esp+14h] [ebp-4h]

  uniquetrans = 0;
  for ( uniqindex = 0; UniqMonst[uniqindex].mtype != -1; ++uniqindex )
  {
    if ( UniqMonst[uniqindex].mlevel == currlevel )
    {
      v2 = 0;
      for ( miniontype = 0; miniontype < nummtypes && !v2; ++miniontype )
        v2 = Monsters[miniontype].mtype == UniqMonst[uniqindex].mtype;
      miniontypea = miniontype - 1;
      if ( !uniqindex && !quests[7]._qactive )
        v2 = 0;
      if ( uniqindex == 2 && !quests[10]._qactive )
        v2 = 0;
      if ( v2 )
        PlaceUniqueMonst(uniqindex, miniontypea, miniontypea, 8);
    }
  }
}

//----- (00402D39) --------------------------------------------------------
void __fastcall monster_402D39(int num)
{
  int mtype; // [esp+10h] [ebp-10h]
  int mtypea; // [esp+10h] [ebp-10h]
  signed int j; // [esp+14h] [ebp-Ch]
  signed int k; // [esp+18h] [ebp-8h]
  int unpackfilesize; // [esp+1Ch] [ebp-4h]

  if ( quests[1]._qlevel == currlevel && quests[1]._qactive == 2 )
  {
    for ( j = 0; j < 112; ++j )
    {
      for ( k = 0; k < 112; ++k )
      {
        if ( dPiece[k][j] == 367 )
        {
          for ( mtype = 0; mtype < nummtypes && Monsters[mtype].mtype != 51; ++mtype )
            ;
          PlaceMonster(num, mtype, k + 1, j + 1);
          ++nummonsters;
        }
      }
    }
  }
  if ( quests[0]._qlevel == currlevel && gbMaxPlayers != 1 )
  {
    for ( mtypea = 0; mtypea < nummtypes && Monsters[mtypea].mtype != UniqMonst[1].mtype; ++mtypea )
      ;
    if ( mtypea >= nummtypes )
      assertion_failed(799, "C:\\Diablo\\Direct\\MONSTER.CPP");
    for ( unpackfilesize = 0; unpackfilesize < nummtypes && !IsSkel(Monsters[unpackfilesize].mtype); ++unpackfilesize )
      ;
    if ( unpackfilesize >= nummtypes )
      assertion_failed(802, "C:\\Diablo\\Direct\\MONSTER.CPP");
    PlaceUniqueMonst(1, mtypea, unpackfilesize, 30);
  }
}

//----- (00402F48) --------------------------------------------------------
void __fastcall PlaceGroup(int mtype, int num, unsigned __int8 leaderf, int leader)
{
  int v4; // eax
  int v5; // eax
  int num_; // [esp+Ch] [ebp-2Ch]
  int yp_; // [esp+14h] [ebp-24h]
  int Xadded_count; // [esp+18h] [ebp-20h]
  int yp; // [esp+1Ch] [ebp-1Ch]
  int Xadded_count2; // [esp+20h] [ebp-18h]
  int xp_; // [esp+24h] [ebp-14h]
  int xp; // [esp+28h] [ebp-10h]
  signed int Xtries; // [esp+30h] [ebp-8h]
  signed int Xtries2; // [esp+34h] [ebp-4h]

  num_ = num;
  Xadded_count = 0;
  Xtries2 = 0;
  do
  {
    while ( Xadded_count )
    {
      --nummonsters;
      --Xadded_count;
      dMonster[monster[nummonsters]._mx][monster[nummonsters]._my] = 0;
    }
    if ( leaderf & 1 )
    {
      v4 = random(92, 8);
      xp = offset_x[v4] + monster[leader]._mx;
      xp_ = offset_x[v4] + monster[leader]._mx;
      yp = offset_y[v4] + monster[leader]._my;
      yp_ = offset_y[v4] + monster[leader]._my;
    }
    else
    {
      do
      {
        v5 = random(93, 80);
        xp = v5 + 16;
        xp_ = v5 + 16;
        yp = random(93, 80) + 16;
        yp_ = yp;
      }
      while ( !MonstPlace(xp, yp) );
    }
    if ( num_ + nummonsters > totalmonsters )
      num_ = totalmonsters - nummonsters;
    Xadded_count2 = 0;
    Xtries = 0;
    while ( Xadded_count2 < num_ && Xtries < 100 )
    {
      if ( !MonstPlace(xp, yp)
        || dung_map[xp_][yp_] != dung_map[xp][yp]
        || leaderf & 2 && (abs(xp - xp_) >= 4 || abs(yp - yp_) >= 4) )
      {
        ++Xtries;
      }
      else
      {
        PlaceMonster(nummonsters, mtype, xp, yp);
        if ( leaderf & 1 )
        {
          monster[nummonsters]._mmaxhp *= 2;
          monster[nummonsters]._mhitpoints = monster[nummonsters]._mmaxhp;
          monster[nummonsters]._mint = monster[leader]._mint;
          if ( leaderf & 2 )
          {
            monster[nummonsters].leader = leader;
            monster[nummonsters].leaderflag = 1;
            monster[nummonsters].mAi = monster[leader].mAi;
          }
        }
        ++nummonsters;
        ++Xadded_count;
        ++Xadded_count2;
      }
      xp += offset_x[random(94, 8)];
      yp += offset_x[random(94, 8)];
    }
    if ( Xadded_count >= num_ )
      break;
    ++Xtries2;
  }
  while ( Xtries2 < 10 );
  if ( leaderf & 2 )
    monster[leader].packsize = Xadded_count;
}

//----- (004032C7) --------------------------------------------------------
void __cdecl InitMonsters()
{
  int num; // [esp+Ch] [ebp-1DCh]
  int mtype; // [esp+10h] [ebp-1D8h]
  int v2; // [esp+14h] [ebp-1D4h]
  int y; // [esp+18h] [ebp-1D0h]
  int x; // [esp+1Ch] [ebp-1CCh]
  int xa; // [esp+1Ch] [ebp-1CCh]
  int xb; // [esp+1Ch] [ebp-1CCh]
  int xc; // [esp+1Ch] [ebp-1CCh]
  int v8; // [esp+20h] [ebp-1C8h]
  signed int j; // [esp+24h] [ebp-1C4h]
  signed int l; // [esp+24h] [ebp-1C4h]
  signed int i; // [esp+28h] [ebp-1C0h]
  signed int k; // [esp+28h] [ebp-1C0h]
  int v13[110]; // [esp+2Ch] [ebp-1BCh]
  int v; // [esp+1E4h] [ebp-4h]

  v = 0;
  v8 = 0;
  for ( x = 16; x < 96; ++x )
  {
    for ( y = 16; y < 96; ++y )
    {
      if ( !SolidLoc(x, y) )
        ++v8;
    }
  }
  v2 = v8 / 30;
  if ( gbMaxPlayers != 1 )
    v2 += v2 >> 1;
  if ( nummonsters + v2 > 190 )
    v2 = 190 - nummonsters;
  totalmonsters = v2 + nummonsters;
  for ( xa = 0; numtrigs > xa; ++xa )
  {
    for ( i = -2; i < 2; ++i )
    {
      for ( j = -2; j < 2; ++j )
        DoVision(i + trigs[xa]._tx, j + trigs[xa]._ty, 15, 0, 0);
    }
  }
  PlaceQuestMonsters();
  monster_402D39(nummonsters);
  for ( xb = 0; xb < nummtypes; ++xb )
  {
    if ( Monsters[xb].mPlaceFlags & 1 )
      v13[v++] = xb;
  }
  while ( totalmonsters > nummonsters )
  {
    mtype = v13[random(95, v)];
    if ( currlevel != 1 && random(95, 2) )
    {
      if ( currlevel == 2 )
        num = random(95, 2) + 2;
      else
        num = random(95, 3) + 3;
    }
    else
    {
      num = 1;
    }
    PlaceGroup(mtype, num, 0, 0);
  }
  for ( xc = 0; numtrigs > xc; ++xc )
  {
    for ( k = -2; k < 2; ++k )
    {
      for ( l = -2; l < 2; ++l )
        DoUnVision(k + trigs[xc]._tx, l + trigs[xc]._ty, 15);
    }
  }
}

//----- (00403653) --------------------------------------------------------
void __fastcall SetMapMonsters(char *pMap, int startx, int starty)
{
  unsigned __int16 v3; // ST30_2
  char *v4; // ST38_4
  int v5; // eax
  int i; // ST1C_4
  signed int j; // [esp+20h] [ebp-1Ch]
  signed int k; // [esp+24h] [ebp-18h]
  unsigned __int16 v10; // [esp+28h] [ebp-14h]
  unsigned __int16 v11; // [esp+2Ch] [ebp-10h]
  char *v12; // [esp+30h] [ebp-Ch]

  v3 = *((_WORD *)pMap + 1);
  v4 = &pMap[2 * *(unsigned __int16 *)pMap * v3 + 4];
  v11 = 2 * *(_WORD *)pMap;
  v10 = 2 * v3;
  v12 = &v4[2 * v11 * v10];
  for ( j = 0; v10 > j; ++j )
  {
    for ( k = 0; v11 > k; ++k )
    {
      if ( *(_WORD *)v12 )
      {
        v5 = AddMonsterType(MonstConvTbl[*(unsigned __int16 *)v12], 2);
        i = nummonsters++;
        PlaceMonster(i, v5, startx + k + 16, starty + j + 16);
      }
      v12 += 2;
    }
  }
}

//----- (0040379D) --------------------------------------------------------
void __fastcall DeleteMonster(int i)
{
  int v1; // ST10_4

  v1 = monstactive[--nummonsters];
  monstactive[nummonsters] = monstactive[i];
  monstactive[i] = v1;
}

//----- (004037EC) --------------------------------------------------------
int __fastcall AddMonster(int x, int y, int dir, int mtype, int InMap)
{
  int i; // [esp+14h] [ebp-4h]

  if ( nummonsters >= 200 )
    return -1;
  i = monstactive[nummonsters++];
  if ( InMap )
    dMonster[x][y] = i + 1;
  InitMonster(i, dir, mtype, x, y);
  return i;
}

//----- (0040387A) --------------------------------------------------------
void __fastcall NewMonsterAnim(int i, AnimStruct *anim, int md)
{
  monster[i]._mAnimData = anim->Frames[md];
  monster[i]._mAnimLen = anim->Rate;
  monster[i]._mAnimFrame = 1;
  monster[i]._mAnimCnt = 0;
  monster[i]._mAnimDelay = anim->Delay;
  monster[i]._mdir = md;
  monster[i]._mFlags &= 0xFFFFFFF9;
}

//----- (00403943) --------------------------------------------------------
int __fastcall M_Enemy(int i, int enemy)
{
  int v2; // ebx
  int v3; // ebx
  int result; // eax
  int Xplridx; // [esp+14h] [ebp-20h]
  int k; // [esp+18h] [ebp-1Ch]
  BOOL v9; // [esp+1Ch] [ebp-18h]
  BOOL v10; // [esp+1Ch] [ebp-18h]
  BOOL v11; // [esp+20h] [ebp-14h]
  signed int Xmindistance; // [esp+24h] [ebp-10h]
  int Xdistance; // [esp+28h] [ebp-Ch]
  int Xdistance2; // [esp+28h] [ebp-Ch]
  int v15; // [esp+2Ch] [ebp-8h]
  signed int j; // [esp+30h] [ebp-4h]

  Xplridx = -1;
  Xmindistance = -1;
  v11 = 0;
  if ( enemy == 1 )
  {
    for ( j = 0; j < 4; ++j )
    {
      if ( plr[j].plractive && plr[j].plrlevel == currlevel && (plr[j]._pHitPoints || gbMaxPlayers == 1) )
      {
        v9 = dung_map[plr[j].WorldX][plr[j].WorldY] == dung_map[monster[i]._mx][monster[i]._my];
        v2 = abs(monster[i]._my - plr[j].WorldY);
        Xdistance = v2 >= abs(monster[i]._mx - plr[j].WorldX) ? abs(monster[i]._my - plr[j].WorldY) : abs(monster[i]._mx - plr[j].WorldX);
        if ( v9 && !v11 || (v9 || !v11) && Xmindistance > Xdistance || Xplridx == -1 )
        {
          monster[i]._mFlags &= 0xFFFFFFEF;
          monster[i]._mFlags &= 0xFFFFFFDF;
          Xplridx = j;
          Xmindistance = Xdistance;
          v11 = v9;
        }
      }
    }
  }
  for ( k = 0; k < nummonsters; ++k )
  {
    v15 = monstactive[k];
    if ( v15 != i && (!enemy || enemy == 1 && monster[v15]._mFlags & 0x20) )
    {
      v10 = dung_map[monster[v15]._mx][monster[v15]._my] == dung_map[monster[i]._mx][monster[i]._my];
      v3 = abs(monster[i]._mx - monster[v15]._mx);
      Xdistance2 = v3 <= abs(monster[i]._my - monster[v15]._my) ? abs(monster[i]._my - monster[v15]._my) : abs(monster[i]._mx - monster[v15]._mx);
      if ( v10 && !v11 || (v10 || !v11) && Xmindistance > Xdistance2 || Xplridx == -1 )
      {
        monster[i]._mFlags |= 0x10u;
        if ( !enemy )
          monster[i]._mFlags |= 0x20u;
        Xplridx = v15;
        Xmindistance = Xdistance2;
        v11 = v10;
      }
    }
  }
  if ( enemy != 1 || Xplridx != -1 )
    result = Xplridx;
  else
    result = monster[i]._menemy;
  return result;
}

//----- (00403ECC) --------------------------------------------------------
int __fastcall M_GetDir(int i, int enemy)
{
  int result; // eax

  if ( monster[i]._mFlags & 0x10 )
    result = GetDirection(monster[i]._mx, monster[i]._my, monster[enemy]._mx, monster[enemy]._my);
  else
    result = GetDirection(monster[i]._mx, monster[i]._my, plr[enemy].WorldX, plr[enemy].WorldY);
  return result;
}

//----- (00403FCA) --------------------------------------------------------
void __fastcall M_CheckEFlag(int i)
{
  int v1; // [esp+10h] [ebp-14h]
  int v2; // [esp+18h] [ebp-Ch]
  signed int j; // [esp+1Ch] [ebp-8h]
  int v4; // [esp+20h] [ebp-4h]

  v1 = monster[i]._mx - 1;
  v4 = monster[i]._my + 1;
  v2 = 0;
  for ( j = 2; j < 10; ++j )
    v2 |= dpiece_defs_map_2[0][16 * v1][16 * v4 + j];
  if ( dArch[v1][v4] | v2 )
    monster[i]._meflag = 1;
  else
    monster[i]._meflag = 0;
}

//----- (004040B7) --------------------------------------------------------
void __fastcall M_StartStand(int i, int md)
{
  int v4; // [esp+14h] [ebp-4h]

  ClearMVars(i);
  NewMonsterAnim(i, monster[i].MType->Anims, md);
  monster[i]._mVar1 = monster[i]._mmode;
  monster[i]._mVar2 = 0;
  monster[i]._mmode = 0;
  monster[i]._mxoff = 0;
  monster[i]._myoff = 0;
  monster[i]._mfutx = monster[i]._mx;
  monster[i]._mfuty = monster[i]._my;
  monster[i]._moldx = monster[i]._mx;
  monster[i]._moldy = monster[i]._my;
  monster[i]._mdir = md;
  M_CheckEFlag(i);
  if ( monster[i]._mFlags & 0x20 )
    v4 = M_Enemy(i, 0);
  else
    v4 = M_Enemy(i, 1);
  monster[i]._menemy = v4;
}

//----- (00404296) --------------------------------------------------------
void __fastcall M_StartDelay(int i, int len)
{
  if ( len > 0 )
  {
    monster[i]._mVar2 = len;
    monster[i]._mmode = 13;
  }
}

//----- (004042E8) --------------------------------------------------------
void __fastcall M_StartSpStand(int i, int md)
{
  NewMonsterAnim(i, &monster[i].MType->Anims[5], md);
  monster[i]._mmode = 11;
  monster[i]._mxoff = 0;
  monster[i]._myoff = 0;
  monster[i]._mfutx = monster[i]._mx;
  monster[i]._mfuty = monster[i]._my;
  monster[i]._moldx = monster[i]._mx;
  monster[i]._moldy = monster[i]._my;
  monster[i]._mdir = md;
  M_CheckEFlag(i);
}

//----- (0040442E) --------------------------------------------------------
void __fastcall M_StartWalk(int i, int xvel, int yvel, int xadd, int yadd, int EndDir)
{
  int v7; // ST1C_4
  int v8; // ST18_4
  int v9; // ST20_4

  v7 = xadd + monster[i]._mx;
  v8 = yadd + monster[i]._my;
  v9 = dPiece[v7][v8] - 1;
  dMonster[v7][v8] = -1 - i;
  monster[i]._mmode = 1;
  monster[i]._moldx = monster[i]._mx;
  monster[i]._moldy = monster[i]._my;
  monster[i]._mfutx = v7;
  monster[i]._mfuty = v8;
  monster[i]._mxvel = xvel;
  monster[i]._myvel = yvel;
  monster[i]._mVar1 = xadd;
  monster[i]._mVar2 = yadd;
  monster[i]._mVar3 = EndDir;
  monster[i]._mdir = EndDir;
  NewMonsterAnim(i, &monster[i].MType->Anims[1], EndDir);
  monster[i]._mVar6 = 0;
  monster[i]._mVar7 = 0;
  monster[i]._mVar8 = 0;
  M_CheckEFlag(i);
}

//----- (0040464A) --------------------------------------------------------
void __fastcall M_StartWalk2(int i, int xvel, int yvel, int xoff, int yoff, int xadd, int yadd, int EndDir)
{
  int v8; // ST1C_4
  int v9; // ST18_4
  int v10; // ST20_4

  v8 = xadd + monster[i]._mx;
  v9 = yadd + monster[i]._my;
  v10 = dPiece[v8][v9] - 1;
  dMonster[monster[i]._mx][monster[i]._my] = -1 - i;
  monster[i]._mVar1 = monster[i]._mx;
  monster[i]._mVar2 = monster[i]._my;
  monster[i]._moldx = monster[i]._mx;
  monster[i]._moldy = monster[i]._my;
  monster[i]._mx = v8;
  monster[i]._my = v9;
  monster[i]._mfutx = v8;
  monster[i]._mfuty = v9;
  dMonster[v8][v9] = i + 1;
  if ( monster[i]._uniqtype )
    ChangeLightXY(monster[i].mlid, monster[i]._mx, monster[i]._my);
  monster[i]._mxoff = xoff;
  monster[i]._myoff = yoff;
  monster[i]._mmode = 2;
  monster[i]._mxvel = xvel;
  monster[i]._myvel = yvel;
  monster[i]._mVar3 = EndDir;
  monster[i]._mdir = EndDir;
  NewMonsterAnim(i, &monster[i].MType->Anims[1], EndDir);
  monster[i]._mVar6 = 16 * xoff;
  monster[i]._mVar7 = 16 * yoff;
  monster[i]._mVar8 = 0;
  M_CheckEFlag(i);
}

//----- (00404989) --------------------------------------------------------
void __fastcall M_StartWalk3(int i, int xvel, int yvel, int xoff, int yoff, int xadd, int yadd, int mapx, int mapy, int EndDir)
{
  int v10; // ST28_4
  int v11; // ST1C_4
  int x; // [esp+14h] [ebp-18h]
  int v15; // [esp+1Ch] [ebp-10h]
  int v16; // [esp+20h] [ebp-Ch]
  int y; // [esp+28h] [ebp-4h]

  v16 = xadd + monster[i]._mx;
  v15 = yadd + monster[i]._my;
  x = mapx + monster[i]._mx;
  y = mapy + monster[i]._my;
  if ( monster[i]._uniqtype )
    ChangeLightXY(monster[i].mlid, x, y);
  v10 = dPiece[v16][v15] - 1;
  v11 = dPiece[x][y] - 1;
  dMonster[monster[i]._mx][monster[i]._my] = -1 - i;
  dMonster[v16][v15] = -1 - i;
  monster[i]._mVar4 = x;
  monster[i]._mVar5 = y;
  dFlags[x][y] |= 0x10u;
  monster[i]._moldx = monster[i]._mx;
  monster[i]._moldy = monster[i]._my;
  monster[i]._mfutx = v16;
  monster[i]._mfuty = v15;
  monster[i]._mxoff = xoff;
  monster[i]._myoff = yoff;
  monster[i]._mmode = 3;
  monster[i]._mxvel = xvel;
  monster[i]._myvel = yvel;
  monster[i]._mVar1 = v16;
  monster[i]._mVar2 = v15;
  monster[i]._mVar3 = EndDir;
  monster[i]._mdir = EndDir;
  NewMonsterAnim(i, &monster[i].MType->Anims[1], EndDir);
  monster[i]._mVar6 = 16 * xoff;
  monster[i]._mVar7 = 16 * yoff;
  monster[i]._mVar8 = 0;
  M_CheckEFlag(i);
}

//----- (00404D06) --------------------------------------------------------
void __fastcall M_StartAttack(int i)
{
  int md; // ST14_4

  md = M_GetDir(i, monster[i]._menemy);
  NewMonsterAnim(i, &monster[i].MType->Anims[2], md);
  monster[i]._mmode = 4;
  monster[i]._mxoff = 0;
  monster[i]._myoff = 0;
  monster[i]._mfutx = monster[i]._mx;
  monster[i]._mfuty = monster[i]._my;
  monster[i]._moldx = monster[i]._mx;
  monster[i]._moldy = monster[i]._my;
  monster[i]._mdir = md;
  M_CheckEFlag(i);
}

//----- (00404E65) --------------------------------------------------------
void __fastcall M_StartRAttack(int i, int missile_type)
{
  int md; // ST18_4

  md = M_GetDir(i, monster[i]._menemy);
  NewMonsterAnim(i, &monster[i].MType->Anims[2], md);
  monster[i]._mmode = 10;
  monster[i]._mVar1 = missile_type;
  monster[i]._mxoff = 0;
  monster[i]._myoff = 0;
  monster[i]._mfutx = monster[i]._mx;
  monster[i]._mfuty = monster[i]._my;
  monster[i]._moldx = monster[i]._mx;
  monster[i]._moldy = monster[i]._my;
  monster[i]._mdir = md;
  M_CheckEFlag(i);
}

//----- (00404FDE) --------------------------------------------------------
void __fastcall M_StartRSpAttack(int i, int missile_type, int dam)
{
  int md; // ST18_4

  md = M_GetDir(i, monster[i]._menemy);
  NewMonsterAnim(i, &monster[i].MType->Anims[5], md);
  monster[i]._mmode = 12;
  monster[i]._mVar1 = missile_type;
  monster[i]._mxoff = 0;
  monster[i]._myoff = 0;
  monster[i]._mfutx = monster[i]._mx;
  monster[i]._mfuty = monster[i]._my;
  monster[i]._moldx = monster[i]._mx;
  monster[i]._moldy = monster[i]._my;
  monster[i]._mdir = md;
  M_CheckEFlag(i);
}

//----- (0040515A) --------------------------------------------------------
void __fastcall M_StartSpAttack(int i)
{
  int md; // ST14_4

  md = M_GetDir(i, monster[i]._menemy);
  NewMonsterAnim(i, &monster[i].MType->Anims[5], md);
  monster[i]._mmode = 7;
  monster[i]._mxoff = 0;
  monster[i]._myoff = 0;
  monster[i]._mfutx = monster[i]._mx;
  monster[i]._mfuty = monster[i]._my;
  monster[i]._moldx = monster[i]._mx;
  monster[i]._moldy = monster[i]._my;
  monster[i]._mdir = md;
  M_CheckEFlag(i);
}

//----- (004052BC) --------------------------------------------------------
void __fastcall M_StartEat(int i)
{
  NewMonsterAnim(i, &monster[i].MType->Anims[5], monster[i]._mdir);
  monster[i]._mmode = 7;
  monster[i]._mxoff = 0;
  monster[i]._myoff = 0;
  monster[i]._mfutx = monster[i]._mx;
  monster[i]._mfuty = monster[i]._my;
  monster[i]._moldx = monster[i]._mx;
  monster[i]._moldy = monster[i]._my;
  M_CheckEFlag(i);
}

//----- (004053F9) --------------------------------------------------------
void __fastcall M_ClearSquares(int i)
{
  int k; // [esp+10h] [ebp-18h]
  int j; // [esp+1Ch] [ebp-Ch]
  int v3; // [esp+20h] [ebp-8h]
  int v4; // [esp+24h] [ebp-4h]

  v4 = monster[i]._moldx;
  v3 = monster[i]._moldy;
  for ( j = v3 - 1; v3 + 1 >= j; ++j )
  {
    for ( k = v4 - 1; v4 + 1 >= k; ++k )
    {
      if ( dMonster[k][j] == -1 - i || dMonster[k][j] == i + 1 )
        dMonster[k][j] = 0;
    }
  }
  dFlags[v4 + 1][v3] &= 0xEFu;
  dFlags[v4][v3 + 1] &= 0xEFu;
}

//----- (00405550) --------------------------------------------------------
void __fastcall M_StartHit(int i, int pnum, int dam)
{
  int mdir; // [esp+14h] [ebp-4h]

  if ( pnum >= 0 )
    monster[i].mWhoHit |= 1 << pnum;
  if ( myplr == pnum )
  {
    delta_monster_hp(i, monster[i]._mhitpoints, currlevel);
    NetSendCmdParam2(0, 0x28u, i, dam);
  }
  PlayEffect(i, 1);
  if ( (signed int)monster[i].MType->mtype >= 29 && (signed int)monster[i].MType->mtype <= 32
    || monster[i].mLevel + 3 <= dam >> 6
    || plr[pnum]._pIFlags & 0x800 )
  {
    if ( pnum >= 0 )
    {
      monster[i]._menemy = pnum;
      monster[i]._mFlags &= 0xFFFFFFEF;
      monster[i]._mdir = M_GetDir(i, pnum);
    }
    if ( monster[i].MType->mtype == 39 )
    {
      M_Teleport(i);
    }
    else if ( (signed int)monster[i].MType->mtype < 16 || (signed int)monster[i].MType->mtype > 19 )
    {
      if ( pnum >= 0 && plr[pnum]._pIFlags & 0x800 )
      {
        mdir = ((unsigned __int8)monster[i]._mdir - 4) & 7;
        if ( DirOK(i, mdir) )
        {
          M_ClearSquares(i);
          monster[i]._moldx += offset_x[mdir];
          monster[i]._moldy += offset_y[mdir];
        }
      }
    }
    else
    {
      monster[i]._mgoal = 1;
    }
    NewMonsterAnim(i, &monster[i].MType->Anims[3], monster[i]._mdir);
    monster[i]._mmode = 5;
    monster[i]._mxoff = 0;
    monster[i]._myoff = 0;
    monster[i]._mx = monster[i]._moldx;
    monster[i]._my = monster[i]._moldy;
    monster[i]._mfutx = monster[i]._mx;
    monster[i]._mfuty = monster[i]._my;
    monster[i]._moldx = monster[i]._mx;
    monster[i]._moldy = monster[i]._my;
    M_CheckEFlag(i);
    M_ClearSquares(i);
    dMonster[monster[i]._mx][monster[i]._my] = i + 1;
  }
}

//----- (004059CA) --------------------------------------------------------
void __fastcall M2MStartHit(int mid, int i, int dam)
{
  delta_monster_hp(mid, monster[mid]._mhitpoints, currlevel);
  NetSendCmdParam2(0, 0x28u, mid, dam);
  PlayEffect(mid, 1);
  if ( (signed int)monster[mid].MType->mtype >= 29 && (signed int)monster[mid].MType->mtype <= 32
    || monster[mid].mLevel + 3 <= dam >> 6 )
  {
    if ( i >= 0 )
      monster[mid]._mdir = ((unsigned __int8)monster[i]._mdir - 4) & 7;
    if ( monster[mid].MType->mtype == 39 )
    {
      M_Teleport(mid);
    }
    else if ( (signed int)monster[mid].MType->mtype >= 16 && (signed int)monster[mid].MType->mtype <= 19 )
    {
      monster[mid]._mgoal = 1;
    }
    if ( monster[mid].MType->mtype != 108 )
    {
      NewMonsterAnim(mid, &monster[mid].MType->Anims[3], monster[mid]._mdir);
      monster[mid]._mmode = 5;
    }
    monster[mid]._mxoff = 0;
    monster[mid]._myoff = 0;
    monster[mid]._mx = monster[mid]._moldx;
    monster[mid]._my = monster[mid]._moldy;
    monster[mid]._mfutx = monster[mid]._mx;
    monster[mid]._mfuty = monster[mid]._my;
    monster[mid]._moldx = monster[mid]._mx;
    monster[mid]._moldy = monster[mid]._my;
    M_CheckEFlag(mid);
    M_ClearSquares(mid);
    dMonster[monster[mid]._mx][monster[mid]._my] = mid + 1;
  }
}

//----- (00405D2F) --------------------------------------------------------
void __fastcall MonstStartKill(int i, int pnum, int sendmsg)
{
  int v3; // ecx
  int md; // [esp+14h] [ebp-4h]

  if ( pnum >= 0 )
    monster[i].mWhoHit |= 1 << pnum;
  AddPlrMonstExper(monster[i].mLevel, (unsigned __int16)monster[i].mExp, monster[i].mWhoHit);
  v3 = monster[i].MType->mtype;
  ++monstkills[v3];
  monster[i]._mhitpoints = 0;
  SetRndSeed(monster[i]._mRndSeed);
  SpawnItem(i, monster[i]._mx, monster[i]._my, sendmsg);
  PlayEffect(i, 2);
  if ( pnum < 0 )
    md = monster[i]._mdir;
  else
    md = M_GetDir(i, pnum);
  monster[i]._mdir = md;
  NewMonsterAnim(i, &monster[i].MType->Anims[4], md);
  monster[i]._mmode = 6;
  monster[i]._mxoff = 0;
  monster[i]._myoff = 0;
  monster[i]._mx = monster[i]._moldx;
  monster[i]._my = monster[i]._moldy;
  monster[i]._mfutx = monster[i]._mx;
  monster[i]._mfuty = monster[i]._my;
  monster[i]._moldx = monster[i]._mx;
  monster[i]._moldy = monster[i]._my;
  M_CheckEFlag(i);
  M_ClearSquares(i);
  dMonster[monster[i]._mx][monster[i]._my] = i + 1;
  QuestStatus(i);
  M_FallenFear(monster[i]._mx, monster[i]._my);
  if ( (signed int)monster[i].MType->mtype >= 46 && (signed int)monster[i].MType->mtype <= 49 )
    AddMissile(monster[i]._mx, monster[i]._my, 0, 0, 0, 59, 1, i, monster[i]._mint + 1);
}

//----- (00406123) --------------------------------------------------------
void __fastcall M2MStartKill(int i, int mid)
{
  int mtype_; // ecx
  int md; // [esp+14h] [ebp-4h]

  delta_kill_monster(mid, monster[mid]._mx, monster[mid]._my, currlevel);
  NetSendCmdLocParam1(0, 0x27u, monster[mid]._mx, monster[mid]._my, mid);
  mtype_ = monster[mid].MType->mtype;
  ++monstkills[mtype_];
  monster[mid]._mhitpoints = 0;
  SetRndSeed(monster[mid]._mRndSeed);
  SpawnItem(mid, monster[mid]._mx, monster[mid]._my, 1u);
  PlayEffect(mid, 2);
  md = ((unsigned __int8)monster[i]._mdir - 4) & 7;
  if ( monster[mid].MType->mtype == 108 )
    md = 0;
  monster[mid]._mdir = md;
  NewMonsterAnim(mid, &monster[mid].MType->Anims[4], md);
  monster[mid]._mmode = 6;
  monster[mid]._mxoff = 0;
  monster[mid]._myoff = 0;
  monster[mid]._mx = monster[mid]._moldx;
  monster[mid]._my = monster[mid]._moldy;
  monster[mid]._mfutx = monster[mid]._mx;
  monster[mid]._mfuty = monster[mid]._my;
  monster[mid]._moldx = monster[mid]._mx;
  monster[mid]._moldy = monster[mid]._my;
  M_CheckEFlag(mid);
  M_ClearSquares(mid);
  dMonster[monster[mid]._mx][monster[mid]._my] = mid + 1;
  QuestStatus(mid);
  M_FallenFear(monster[mid]._mx, monster[mid]._my);
  if ( (signed int)monster[mid].MType->mtype >= 46 && (signed int)monster[mid].MType->mtype <= 49 )
    AddMissile(monster[mid]._mx, monster[mid]._my, 0, 0, 0, 59, 1, mid, monster[mid]._mint + 1);
}

//----- (0040650E) --------------------------------------------------------
void __fastcall M_StartKill(int i, int pnum)
{
  if ( pnum == myplr )
  {
    delta_kill_monster(i, monster[i]._mx, monster[i]._my, currlevel);
    NetSendCmdLocParam1(0, 0x27u, monster[i]._mx, monster[i]._my, i);
  }
  MonstStartKill(i, pnum, 1);
}

//----- (004065B3) --------------------------------------------------------
void __fastcall M_SyncStartKill(int i, int x, int y, int pnum)
{
  if ( monster[i]._mhitpoints )
  {
    if ( monster[i]._mmode != 6 )
    {
      monster[i]._mx = x;
      monster[i]._my = y;
      MonstStartKill(i, pnum, 0);
    }
  }
}

//----- (00406649) --------------------------------------------------------
void __fastcall M_StartFadein(int i, int md)
{
  NewMonsterAnim(i, &monster[i].MType->Anims[5], md);
  monster[i]._mmode = 8;
  monster[i]._mxoff = 0;
  monster[i]._myoff = 0;
  monster[i]._mfutx = monster[i]._mx;
  monster[i]._mfuty = monster[i]._my;
  monster[i]._moldx = monster[i]._mx;
  monster[i]._moldy = monster[i]._my;
  M_CheckEFlag(i);
  monster[i]._mdir = md;
  monster[i]._mFlags |= 1u;
}

//----- (004067A4) --------------------------------------------------------
void __fastcall M_StartFadeout(int i, int md, unsigned __int8 backwards)
{
  NewMonsterAnim(i, &monster[i].MType->Anims[5], md);
  monster[i]._mAnimFrame = monster[i].MType->Anims[5].Rate;
  monster[i]._mmode = 9;
  monster[i]._mxoff = 0;
  monster[i]._myoff = 0;
  monster[i]._mfutx = monster[i]._mx;
  monster[i]._mfuty = monster[i]._my;
  monster[i]._moldx = monster[i]._mx;
  monster[i]._moldy = monster[i]._my;
  M_CheckEFlag(i);
  monster[i]._mdir = md;
  monster[i]._mFlags |= 2u;
}

//----- (0040692D) --------------------------------------------------------
void __fastcall M_StartHeal(int i)
{
  MonsterStruct *v1; // ST10_4

  v1 = &monster[i];
  monster[i]._mAnimData = monster[i].MType->Anims[5].Frames[monster[i]._mdir];
  v1->_mAnimFrame = v1->MType->Anims[5].Rate;
  v1->_mFlags |= 2u;
  v1->_mmode = 16;
  v1->_mVar1 = monster[i]._mmaxhp / (16 * (random(97, 5) + 4));
}

//----- (004069C7) --------------------------------------------------------
void __fastcall M_ChangeLightOffset(int monst)
{
  int y; // [esp+10h] [ebp-Ch]
  int x; // [esp+14h] [ebp-8h]
  int xa; // [esp+14h] [ebp-8h]
  signed int v4; // [esp+18h] [ebp-4h]
  signed int v5; // [esp+18h] [ebp-4h]

  x = monster[monst]._mxoff + 2 * monster[monst]._myoff;
  y = 2 * monster[monst]._myoff - monster[monst]._mxoff;
  if ( x >= 0 )
  {
    v4 = 1;
  }
  else
  {
    v4 = -1;
    x = -x;
  }
  xa = (x >> 3) * v4;
  if ( y >= 0 )
  {
    v5 = 1;
  }
  else
  {
    v5 = -1;
    y = monster[monst]._mxoff - 2 * monster[monst]._myoff;
  }
  ChangeLightOff(monster[monst].mlid, xa, (y >> 3) * v5);
}

//----- (00406ABF) --------------------------------------------------------
int __fastcall M_DoStand(int i)
{
  int v1; // eax

  if ( monster[i]._mAnimLen == monster[i]._mAnimFrame )
  {
    if ( monster[i]._mFlags & 0x20 )
      v1 = M_Enemy(i, 0);
    else
      v1 = M_Enemy(i, 1);
    monster[i]._menemy = v1;
  }
  ++monster[i]._mVar2;
  return 0;
}

//----- (00406B78) --------------------------------------------------------
int __fastcall M_DoWalk(int i)
{
  int v3; // [esp+10h] [ebp-4h]

  if ( monster[i].MType->Anims[1].Rate == monster[i]._mVar8 )
  {
    dMonster[monster[i]._mx][monster[i]._my] = 0;
    monster[i]._mx += monster[i]._mVar1;
    monster[i]._my += monster[i]._mVar2;
    dMonster[monster[i]._mx][monster[i]._my] = i + 1;
    if ( monster[i]._uniqtype )
      ChangeLightXY(monster[i].mlid, monster[i]._mx, monster[i]._my);
    M_StartStand(i, monster[i]._mdir);
    v3 = 1;
  }
  else
  {
    if ( !monster[i]._mAnimCnt )
    {
      ++monster[i]._mVar8;
      monster[i]._mVar6 += monster[i]._mxvel;
      monster[i]._mVar7 += monster[i]._myvel;
      monster[i]._mxoff = monster[i]._mVar6 >> 4;
      monster[i]._myoff = monster[i]._mVar7 >> 4;
    }
    v3 = 0;
  }
  if ( monster[i]._uniqtype )
    M_ChangeLightOffset(i);
  return v3;
}

//----- (00406E1A) --------------------------------------------------------
int __fastcall M_DoWalk2(int i)
{
  int v3; // [esp+10h] [ebp-4h]

  if ( monster[i].MType->Anims[1].Rate == monster[i]._mVar8 )
  {
    dMonster[monster[i]._mVar1][monster[i]._mVar2] = 0;
    if ( monster[i]._uniqtype )
      ChangeLightXY(monster[i].mlid, monster[i]._mx, monster[i]._my);
    M_StartStand(i, monster[i]._mdir);
    v3 = 1;
  }
  else
  {
    if ( !monster[i]._mAnimCnt )
    {
      ++monster[i]._mVar8;
      monster[i]._mVar6 += monster[i]._mxvel;
      monster[i]._mVar7 += monster[i]._myvel;
      monster[i]._mxoff = monster[i]._mVar6 >> 4;
      monster[i]._myoff = monster[i]._mVar7 >> 4;
    }
    v3 = 0;
  }
  if ( monster[i]._uniqtype )
    M_ChangeLightOffset(i);
  return v3;
}

//----- (0040702F) --------------------------------------------------------
int __fastcall M_DoWalk3(int i)
{
  int v3; // [esp+10h] [ebp-4h]

  if ( monster[i].MType->Anims[1].Rate == monster[i]._mVar8 )
  {
    dMonster[monster[i]._mx][monster[i]._my] = 0;
    monster[i]._mx = monster[i]._mVar1;
    monster[i]._my = monster[i]._mVar2;
    dFlags[monster[i]._mVar4][monster[i]._mVar5] &= 0xEFu;
    dMonster[monster[i]._mx][monster[i]._my] = i + 1;
    if ( monster[i]._uniqtype )
      ChangeLightXY(monster[i].mlid, monster[i]._mx, monster[i]._my);
    M_StartStand(i, monster[i]._mdir);
    v3 = 1;
  }
  else
  {
    if ( !monster[i]._mAnimCnt )
    {
      ++monster[i]._mVar8;
      monster[i]._mVar6 += monster[i]._mxvel;
      monster[i]._mVar7 += monster[i]._myvel;
      monster[i]._mxoff = monster[i]._mVar6 >> 4;
      monster[i]._myoff = monster[i]._mVar7 >> 4;
    }
    v3 = 0;
  }
  if ( monster[i]._uniqtype )
    M_ChangeLightOffset(i);
  return v3;
}

//----- (00407346) --------------------------------------------------------
void __fastcall M_TryM2MHit(int i, int mid, int hper, int mind, int maxd)
{
  int dam; // [esp+14h] [ebp-8h]
  int v8; // [esp+18h] [ebp-4h]

  if ( (signed int)(monster[mid]._mhitpoints & 0xFFFFFFC0) > 0
    && (monster[mid].MType->mtype != 32 || monster[mid]._mgoal != 2) )
  {
    v8 = random(4, 100);
    if ( monster[mid]._mmode == 15 )
      v8 = 0;
    if ( monster[mid]._mFlags & 4 )
    {
      monster[mid]._mFlags &= 0xFFFFFFFB;
      monster[mid]._mmode = 7;
    }
    else if ( hper > v8 )
    {
      dam = (random(5, maxd - mind + 1) + mind) << 6;
      monster[mid]._mhitpoints -= dam;
      if ( monster[mid]._mmode == 15 )
        monster[mid]._mhitpoints = 0;
      if ( (signed int)(monster[mid]._mhitpoints & 0xFFFFFFC0) > 0 )
        M2MStartHit(mid, i, dam);
      else
        M2MStartKill(i, mid);
    }
  }
}

//----- (00407511) --------------------------------------------------------
void __fastcall M_TryH2HHit(int i, int pnum, int Hit, int MinDam, int MaxDam)
{
  int v5; // ST24_4
  int dir; // ST28_4
  int v9; // [esp+14h] [ebp-28h]
  int v; // [esp+20h] [ebp-1Ch]
  int v11; // [esp+24h] [ebp-18h]
  int v12; // [esp+28h] [ebp-14h]
  int dam; // [esp+30h] [ebp-Ch]
  int v14; // [esp+34h] [ebp-8h]
  int v15; // [esp+38h] [ebp-4h]

  if ( monster[i]._mFlags & 0x10 )
  {
    M_TryM2MHit(i, pnum, Hit, MinDam, MaxDam);
  }
  else if ( (signed int)(plr[pnum]._pHitPoints & 0xFFFFFFC0) > 0
         && !plr[pnum]._pInvincible
         && !(plr[pnum]._pSpellFlags & 1) )
  {
    v5 = abs(monster[i]._mx - plr[pnum].WorldX);
    v9 = abs(monster[i]._my - plr[pnum].WorldY);
    if ( v5 < 2 && v9 < 2 )
    {
      v15 = random(98, 100);
      v12 = Hit
          + 30
          + 2 * (monster[i].mLevel - plr[pnum]._pLevel)
          - (plr[pnum]._pIAC
           + plr[pnum]._pIBonusAC
           + plr[pnum]._pArmorClass);
      if ( v12 < 10 )
        v12 = 10;
      if ( plr[pnum]._pmode && plr[pnum]._pmode != 4 || !plr[pnum]._pBlockFlag )
        v14 = 100;
      else
        v14 = random(98, 100);
      v11 = plr[pnum]._pBaseToBlk + plr[pnum]._pDexterity - 2 * (monster[i].mLevel - plr[pnum]._pLevel);
      if ( v11 < 0 )
        v11 = 0;
      if ( v11 > 100 )
        v11 = 100;
      if ( v12 > v15 )
      {
        if ( v11 <= v14 )
        {
          v = (plr[pnum]._pIGetHit << 6) + (MinDam << 6) + random(99, (MaxDam - MinDam + 1) << 6);
          if ( v < 0 )
            v = 0;
          if ( pnum == myplr )
          {
            plr[pnum]._pHitPoints -= v;
            plr[pnum]._pHPBase -= v;
          }
          if ( monster[i].MType->mtype == 3 && pnum == myplr && plr[pnum]._pMaxHP > 64 )
          {
            plr[pnum]._pMaxHP -= 64;
            if ( plr[pnum]._pHitPoints > plr[pnum]._pMaxHP )
              plr[pnum]._pHitPoints = plr[pnum]._pMaxHP;
            plr[pnum]._pMaxHPBase -= 64;
            if ( plr[pnum]._pMaxHPBase < plr[pnum]._pHPBase )
              plr[pnum]._pHPBase = plr[pnum]._pMaxHPBase;
          }
          if ( plr[pnum]._pIFlags & 0x4000000 )
          {
            dam = (random(99, 3) + 1) << 6;
            monster[i]._mhitpoints -= dam;
            if ( (signed int)(monster[i]._mhitpoints & 0xFFFFFFC0) > 0 )
              M_StartHit(i, pnum, dam);
            else
              M_StartKill(i, pnum);
          }
          if ( !(monster[i]._mFlags & 0x1000) && monster[i].MType->mtype == 50 )
            monster[i]._mhitpoints += v;
          if ( plr[pnum]._pHitPoints > plr[pnum]._pMaxHP )
          {
            plr[pnum]._pHitPoints = plr[pnum]._pMaxHP;
            plr[pnum]._pHPBase = plr[pnum]._pMaxHPBase;
          }
          if ( (signed int)(plr[pnum]._pHitPoints & 0xFFFFFFC0) > 0 )
          {
            StartPlrHit(pnum, v);
          }
          else
          {
            plr[pnum]._pHitPoints = 0;
            SyncPlrKill(pnum);
          }
        }
        else
        {
          dir = GetDirection(plr[pnum].WorldX, plr[pnum].WorldY, monster[i]._mx, monster[i]._my);
          StartPlrBlock(pnum, dir);
        }
      }
    }
  }
}

//----- (00407D29) --------------------------------------------------------
int __fastcall M_DoAttack(int i)
{
  if ( monster[i].MData->mAFNum == monster[i]._mAnimFrame )
  {
    M_TryH2HHit(i, monster[i]._menemy, (unsigned __int8)monster[i].mHit, monster[i].mMinDamage, monster[i].mMaxDamage);
    PlayEffect(i, 0);
  }
  if ( (signed int)monster[i].MType->mtype >= 60
    && (signed int)monster[i].MType->mtype <= 63
    && monster[i]._mAnimFrame == 9 )
  {
    M_TryH2HHit(
      i,
      monster[i]._menemy,
      (unsigned __int8)monster[i].mHit + 10,
      monster[i].mMinDamage - 2,
      monster[i].mMaxDamage - 2);
    PlayEffect(i, 0);
  }
  if ( (signed int)monster[i].MType->mtype >= 76
    && (signed int)monster[i].MType->mtype <= 79
    && monster[i]._mAnimFrame == 13 )
  {
    M_TryH2HHit(
      i,
      monster[i]._menemy,
      (unsigned __int8)monster[i].mHit - 20,
      monster[i].mMinDamage + 4,
      monster[i].mMaxDamage + 4);
    PlayEffect(i, 0);
  }
  if ( monster[i].MType->Anims[2].Rate != monster[i]._mAnimFrame )
    return 0;
  M_StartStand(i, monster[i]._mdir);
  return 1;
}

//----- (00407FE0) --------------------------------------------------------
int __fastcall M_DoRAttack(int i)
{
  int v3; // [esp+10h] [ebp-4h]

  v3 = monster[i]._menemy;
  if ( monster[i].MData->mAFNum == monster[i]._mAnimFrame )
  {
    if ( monster[i]._mFlags & 0x10 )
      AddMissile(
        monster[i]._mx,
        monster[i]._my,
        monster[v3]._mx,
        monster[v3]._my,
        monster[i]._mdir,
        monster[i]._mVar1,
        1,
        i,
        4);
    else
      AddMissile(
        monster[i]._mx,
        monster[i]._my,
        plr[v3].WorldX,
        plr[v3].WorldY,
        monster[i]._mdir,
        monster[i]._mVar1,
        1,
        i,
        4);
    PlayEffect(i, 0);
  }
  if ( monster[i].MType->Anims[2].Rate != monster[i]._mAnimFrame )
    return 0;
  M_StartStand(i, monster[i]._mdir);
  return 1;
}

//----- (004081EF) --------------------------------------------------------
int __fastcall M_DoRSpAttack(int i)
{
  int v3; // [esp+10h] [ebp-8h]

  v3 = monster[i]._menemy;
  if ( monster[i].MData->mAFNum2 == monster[i]._mAnimFrame )
  {
    if ( monster[i]._mFlags & 0x10 )
      AddMissile(
        monster[i]._mx,
        monster[i]._my,
        monster[v3]._mx,
        monster[v3]._my,
        monster[i]._mdir,
        monster[i]._mVar1,
        1,
        i,
        4);
    else
      AddMissile(
        monster[i]._mx,
        monster[i]._my,
        plr[v3].WorldX,
        plr[v3].WorldY,
        monster[i]._mdir,
        monster[i]._mVar1,
        1,
        i,
        4);
    PlayEffect(i, 3);
  }
  if ( monster[i].MType->Anims[5].Rate != monster[i]._mAnimFrame )
    return 0;
  M_StartStand(i, monster[i]._mdir);
  return 1;
}

//----- (00408407) --------------------------------------------------------
int __fastcall M_DoSAttack(int i)
{
  if ( monster[i].MData->mAFNum2 == monster[i]._mAnimFrame )
    M_TryH2HHit(i, monster[i]._menemy, monster[i].mHit2, monster[i].mMinDamage2, monster[i].mMaxDamage2);
  if ( monster[i].MType->Anims[5].Rate != monster[i]._mAnimFrame )
    return 0;
  M_StartStand(i, monster[i]._mdir);
  return 1;
}

//----- (00408512) --------------------------------------------------------
int __fastcall M_DoFadein(int i)
{
  if ( monster[i]._mAnimLen != monster[i]._mAnimFrame )
    return 0;
  M_StartStand(i, monster[i]._mdir);
  return 1;
}

//----- (00408583) --------------------------------------------------------
int __fastcall M_DoFadeout(int i)
{
  signed int v2; // [esp+10h] [ebp-4h]

  if ( monster[i]._mAnimFrame != 1 )
    return 0;
  v2 = monster[i].MType->mtype;
  if ( v2 < 72 || v2 > 75 )
    monster[i]._mFlags &= 0xFFFFFFFC;
  else
    monster[i]._mFlags &= 0xFFFFFFFD;
  M_StartStand(i, monster[i]._mdir);
  return 1;
}

//----- (0040863F) --------------------------------------------------------
int __fastcall M_DoHeal(int i)
{
  MonsterStruct *v2; // [esp+10h] [ebp-4h]

  v2 = &monster[i];
  if ( monster[i]._mFlags & 8 )
    return 0;
  if ( v2->_mAnimFrame == 1 )
  {
    v2->_mFlags &= 0xFFFFFFFD;
    v2->_mFlags |= 4u;
    if ( v2->_mhitpoints + v2->_mVar1 >= v2->_mmaxhp )
    {
      v2->_mhitpoints = v2->_mmaxhp;
      v2->_mFlags &= 0xFFFFFFFB;
      v2->_mmode = 7;
    }
    else
    {
      v2->_mhitpoints += v2->_mVar1;
    }
  }
  return 0;
}

//----- (0040870A) --------------------------------------------------------
int __fastcall M_DoTalk(int i)
{
  MonsterStruct *v1; // ST24_4

  v1 = &monster[i];
  InitQTextMsg(monster[i].mtalkmsg);
  M_StartStand(i, monster[i]._mdir);
  v1->_mgoal = 8;
  if ( monster[i].mName == UniqMonst[0].mName )
  {
    if ( monster[i].mtalkmsg == 19 )
      quests[7]._qactive = 2;
    if ( monster[i].mtalkmsg == 20 && !(monster[i]._mFlags & 0x40) )
    {
      SpawnItem(i, monster[i]._mx + 1, monster[i]._my + 1, 1u);
      monster[i]._mFlags |= 0x40u;
    }
  }
  if ( monster[i].mName == UniqMonst[2].mName && monster[i].mtalkmsg == 25 )
  {
    quests[10]._qactive = 2;
    CreateTypeItem(monster[i]._mx + 1, monster[i]._my + 1, 0, 0, 24, 1, 0);
  }
  return 0;
}

//----- (004088B3) --------------------------------------------------------
void __fastcall M_Teleport(int i)
{
  MonsterStruct *v2; // [esp+10h] [ebp-2Ch]
  int v3; // [esp+14h] [ebp-28h]
  int y; // [esp+18h] [ebp-24h]
  int x; // [esp+1Ch] [ebp-20h]
  signed int v6; // [esp+20h] [ebp-1Ch]
  int v7; // [esp+24h] [ebp-18h]
  int v8; // [esp+28h] [ebp-14h]
  int enemy_; // [esp+2Ch] [ebp-10h]
  signed int k; // [esp+30h] [ebp-Ch]
  signed int j; // [esp+34h] [ebp-8h]
  int v12; // [esp+38h] [ebp-4h]

  v2 = &monster[i];
  v6 = 0;
  enemy_ = monster[i]._menemy;
  if ( monster[i]._mFlags & 0x10 )
  {
    v3 = monster[enemy_]._mx;
    v12 = monster[enemy_]._my;
  }
  else
  {
    v3 = plr[enemy_].WorldX;
    v12 = plr[enemy_].WorldY;
  }
  v8 = 2 * random(100, 2) - 1;
  v7 = 2 * random(100, 2) - 1;
  for ( j = -1; j <= 1 && !v6; ++j )
  {
    for ( k = -1; k < 1 && !v6; ++k )
    {
      if ( j || k )
      {
        x = v3 + v8 * j;
        y = v12 + v7 * k;
        if ( y >= 0 && y < 112 && x >= 0 && x < 112 && v2->_mx != x && v2->_my != y )
        {
          if ( PosOkMonst(i, x, y) )
            v6 = 1;
        }
      }
    }
  }
  if ( v6 )
  {
    M_ClearSquares(i);
    dMonster[v2->_mx][v2->_my] = 0;
    dMonster[x][y] = i + 1;
    v2->_moldx = x;
    v2->_moldy = y;
    v2->_mdir = M_GetDir(i, enemy_);
    M_CheckEFlag(i);
  }
}

//----- (00408AF9) --------------------------------------------------------
int __fastcall M_DoGotHit(int i)
{
  if ( monster[i].MType->Anims[3].Rate != monster[i]._mAnimFrame )
    return 0;
  M_StartStand(i, monster[i]._mdir);
  return 1;
}

//----- (00408B70) --------------------------------------------------------
void __fastcall M_UpdateLeader(int i)
{
  int v1; // ecx
  int j; // [esp+10h] [ebp-8h]
  int v3; // [esp+14h] [ebp-4h]

  if ( monster[i]._uniqtype && UniqMonst[monster[i]._uniqtype - 1].mUnqAttr & 2 )
  {
    for ( j = 0; nummonsters > j; ++j )
    {
      v3 = monstactive[j];
      if ( monster[v3].leaderflag == 1 && monster[v3].leader == i )
        monster[v3].leaderflag = 0;
    }
  }
  if ( monster[i].leaderflag == 1 )
  {
    v1 = monster[i].leader;
    --monster[v1].packsize;
  }
}

//----- (00408C98) --------------------------------------------------------
int __fastcall M_DoDeath(int i)
{
  int ia; // [esp+Ch] [ebp-4h]

  ia = i;
  if ( monster[i].MType->Anims[4].Rate == monster[i]._mAnimFrame )
  {
    if ( monster[i]._uniqtype )
      AddDead(monster[i]._mx, monster[i]._my, monster[i]._udeadval, monster[i]._mdir);
    else
      AddDead(monster[i]._mx, monster[i]._my, monster[i].MType->mdeadval, monster[i]._mdir);
    dMonster[monster[ia]._mx][monster[ia]._my] = 0;
    monster[ia]._mDelFlag = 1;
    M_UpdateLeader(ia);
  }
  return 0;
}

//----- (00408E18) --------------------------------------------------------
int __fastcall M_DoSpStand(int i)
{
  if ( monster[i].MData->mAFNum2 == monster[i]._mAnimFrame )
    PlayEffect(i, 3);
  if ( monster[i]._mAnimFrame != monster[i]._mAnimLen )
    return 0;
  M_StartStand(i, monster[i]._mdir);
  return 1;
}

//----- (00408EC9) --------------------------------------------------------
int __fastcall M_DoDelay(int i)
{
  int v1; // ST20_4
  int v2; // eax
  int v3; // ST20_4
  int v4; // eax
  int v5; // ST10_4
  int enemy_; // [esp+1Ch] [ebp-8h]

  enemy_ = monster[i]._menemy;
  if ( monster[i]._mFlags & 0x10 )
  {
    v3 = monster[i]._mx - monster[enemy_]._mfutx;
    v4 = monster[i]._my - monster[enemy_]._mfuty;
  }
  else
  {
    v1 = monster[i]._mx - plr[enemy_]._px;
    v2 = monster[i]._my - plr[enemy_]._py;
  }
  monster[i]._mAnimData = monster[i].MType->Anims[0].Frames[M_GetDir(i, enemy_)];
  v5 = monster[i]._mVar2--;
  if ( v5 )
    return 0;
  M_StartStand(i, monster[i]._mdir);
  return 1;
}

//----- (00409073) --------------------------------------------------------
int __fastcall M_DoStone(int i)
{
  if ( !monster[i]._mhitpoints )
  {
    dMonster[monster[i]._mx][monster[i]._my] = 0;
    monster[i]._mDelFlag = 1;
  }
  return 0;
}

//----- (004090FB) --------------------------------------------------------
void __fastcall M_WalkDir(int i, int md)
{
  int Xrate; // [esp+18h] [ebp-4h]

  Xrate = monster[i].MType->Anims[1].Rate - 1;
  switch ( md )
  {
    case 0:
      M_StartWalk2(i, 0, MWVel[Xrate][1], 0, -32, 1, 1, 0);
      break;
    case 1:
      M_StartWalk2(i, -MWVel[Xrate][1], MWVel[Xrate][0], 32, -16, 0, 1, 1);
      break;
    case 2:
      M_StartWalk3(i, -MWVel[Xrate][2], 0, 32, -16, -1, 1, 0, 1, 2);
      break;
    case 3:
      M_StartWalk(i, -MWVel[Xrate][1], -MWVel[Xrate][0], -1, 0, 3);
      break;
    case 4:
      M_StartWalk(i, 0, -MWVel[Xrate][1], -1, -1, 4);
      break;
    case 5:
      M_StartWalk(i, MWVel[Xrate][1], -MWVel[Xrate][0], 0, -1, 5);
      break;
    case 6:
      M_StartWalk3(i, MWVel[Xrate][2], 0, -32, -16, 1, -1, 1, 0, 6);
      break;
    case 7:
      M_StartWalk2(i, MWVel[Xrate][1], MWVel[Xrate][0], -32, -16, 1, 0, 7);
      break;
    default:
      return;
  }
}

//----- (004092E3) --------------------------------------------------------
void __fastcall GroupUnity(int i)
{
  int j; // [esp+10h] [ebp-Ch]
  BOOL v3; // [esp+14h] [ebp-8h]
  int v4; // [esp+14h] [ebp-8h]
  int v5; // [esp+18h] [ebp-4h]

  if ( monster[i].leaderflag )
  {
    v5 = monster[i].leader;
    v3 = LineClearF(
           (BOOL (__fastcall *)(int, int))CheckNoSolid,
           monster[i]._mx,
           monster[i]._my,
           monster[v5]._mfutx,
           monster[v5]._mfuty);
    if ( v3 || monster[i].leaderflag != 1 )
    {
      if ( v3
        && monster[i].leaderflag == 2
        && abs(monster[i]._mx - monster[v5]._mfutx) < 4
        && abs(monster[i]._my - monster[v5]._mfuty) < 4 )
      {
        ++monster[v5].packsize;
        monster[i].leaderflag = 1;
      }
    }
    else
    {
      --monster[v5].packsize;
      monster[i].leaderflag = 2;
    }
  }
  if ( monster[i].leaderflag == 1 )
  {
    if ( monster[v5]._msquelch < (signed int)monster[i]._msquelch )
    {
      monster[v5]._lastx = monster[i]._mx;
      monster[v5]._lasty = monster[i]._my;
      monster[v5]._msquelch = monster[i]._msquelch - 1;
    }
  }
  else if ( monster[i]._uniqtype && UniqMonst[monster[i]._uniqtype - 1].mUnqAttr & 2 )
  {
    for ( j = 0; nummonsters > j; ++j )
    {
      v4 = monstactive[j];
      if ( monster[v4].leaderflag == 1
        && monster[v4].leader == i
        && monster[v4]._msquelch < (signed int)monster[i]._msquelch )
      {
        monster[v4]._lastx = monster[i]._mx;
        monster[v4]._lasty = monster[i]._my;
        monster[v4]._msquelch = monster[i]._msquelch - 1;
      }
    }
  }
}

//----- (004096E6) --------------------------------------------------------
BOOL __fastcall M_CallWalk(int i, int md)
{
  int mdir; // [esp+Ch] [ebp-10h]
  BOOL v5; // [esp+14h] [ebp-8h]
  BOOL v6; // [esp+14h] [ebp-8h]
  BOOL v7; // [esp+14h] [ebp-8h]

  mdir = md;
  v5 = DirOK(i, md);
  if ( random(101, 2) )
    v6 = v5 || (mdir = left[md], DirOK(i, left[md])) || (mdir = right[md], DirOK(i, right[md]));
  else
    v6 = v5 || (mdir = right[md], DirOK(i, right[md])) || (mdir = left[md], DirOK(i, left[md]));
  if ( random(102, 2) )
    v7 = v6
      || (mdir = right[right[md]], DirOK(i, right[right[md]]))
      || (mdir = left[left[md]], DirOK(i, left[left[md]]));
  else
    v7 = v6
      || (mdir = left[left[md]], DirOK(i, left[left[md]]))
      || (mdir = right[right[md]], DirOK(i, right[right[md]]));
  if ( v7 )
    M_WalkDir(i, mdir);
  return v7;
}

//----- (004098FA) --------------------------------------------------------
BOOL __fastcall M_PathWalk(int i)
{
  int v1; // edx MAPDST
  BOOL v5; // [esp+14h] [ebp-4h]

  v5 = DirOK(i, v1);
  if ( v5 )
    M_WalkDir(i, v1);
  return v5;
}

//----- (00409939) --------------------------------------------------------
BOOL __fastcall M_RoundWalk(int i, int md, int *dir)
{
  int mdir; // [esp+Ch] [ebp-10h]
  BOOL v6; // [esp+14h] [ebp-8h]
  int v7; // [esp+18h] [ebp-4h]

  if ( *dir )
    mdir = left[left[md]];
  else
    mdir = right[right[md]];
  v7 = mdir;
  v6 = DirOK(i, mdir);
  if ( !v6 )
  {
    if ( *dir )
    {
      mdir = right[mdir];
      v6 = DirOK(i, right[v7]) || (mdir = right[right[v7]], DirOK(i, right[right[v7]]));
    }
    else
    {
      mdir = left[mdir];
      v6 = DirOK(i, left[v7]) || (mdir = left[left[v7]], DirOK(i, left[left[v7]]));
    }
  }
  if ( v6 )
  {
    M_WalkDir(i, mdir);
  }
  else
  {
    if ( *dir )
      *dir = 0;
    else
      *dir = 1;
    v6 = M_CallWalk(i, opposite[v7]);
  }
  return v6;
}

//----- (00409B93) --------------------------------------------------------
void __fastcall MAI_Zombie(int i)
{
  int v1; // eax
  int md; // ST18_4
  int v3; // ebx
  int v4; // ebx
  int v5; // ST18_4
  int v6; // ebx
  MonsterStruct *v8; // [esp+10h] [ebp-18h]
  int v9; // [esp+18h] [ebp-10h]
  int v10; // [esp+1Ch] [ebp-Ch]
  int v11; // [esp+1Ch] [ebp-Ch]
  int enemy; // [esp+20h] [ebp-8h]
  int v13; // [esp+24h] [ebp-4h]
  int v14; // [esp+24h] [ebp-4h]

  v8 = &monster[i];
  enemy = monster[i]._menemy;
  if ( monster[i]._mmode == MM_STAND )
  {
    v13 = v8->_mx;
    v10 = v8->_my;
    if ( dFlags[v13][v10] & 2 )
    {
      if ( v8->_mFlags & 0x10 )
      {
        v14 = v13 - monster[enemy]._mfutx;
        v1 = monster[enemy]._mfuty;
      }
      else
      {
        v14 = v13 - plr[enemy]._px;
        v1 = plr[enemy]._py;
      }
      v11 = v10 - v1;
      md = v8->_mdir;
      v9 = random(103, 100);
      if ( abs(v14) >= 2 || abs(v11) >= 2 )
      {
        if ( 2 * v8->_mint + 10 > v9 )
        {
          v3 = 2 * v8->_mint + 4;
          if ( v3 <= abs(v14) || (v4 = 2 * v8->_mint + 4, v4 <= abs(v11)) )
          {
            v6 = 2 * v8->_mint + 20;
            if ( v6 > random(104, 100) )
              random(104, 8);
            M_PathWalk(i);
          }
          else
          {
            v5 = M_GetDir(i, enemy);
            M_CallWalk(i, v5);
          }
        }
      }
      else if ( 2 * v8->_mint + 10 > v9 )
      {
        M_StartAttack(i);
      }
      if ( v8->_mmode == MM_STAND )
        v8->_mAnimData = v8->MType->Anims[0].Frames[v8->_mdir];
    }
  }
}

//----- (00409DFA) --------------------------------------------------------
void __fastcall MAI_SkelSd(int i)
{
  int v1; // eax
  int v2; // ebx
  int v3; // eax
  int v4; // ebx
  int v5; // eax
  MonsterStruct *v7; // [esp+10h] [ebp-14h]
  int md; // [esp+14h] [ebp-10h]
  int v9; // [esp+18h] [ebp-Ch]
  int v10; // [esp+1Ch] [ebp-8h]
  int v11; // [esp+20h] [ebp-4h]

  v7 = &monster[i];
  v10 = monster[i]._menemy;
  if ( monster[i]._mmode == MM_STAND && v7->_msquelch )
  {
    if ( v7->_mFlags & 0x10 )
    {
      v11 = v7->_mx - monster[v10]._mfutx;
      v1 = v7->_my - monster[v10]._mfuty;
    }
    else
    {
      v11 = v7->_mx - plr[v10]._px;
      v1 = v7->_my - plr[v10]._py;
    }
    v9 = v1;
    md = GetDirection(v7->_mx, v7->_my, v7->_lastx, v7->_lasty);
    v7->_mdir = md;
    if ( abs(v11) >= 2 || abs(v9) >= 2 )
    {
      if ( v7->_mVar1 == 13 || (v4 = 35 - 4 * v7->_mint, v4 <= random(106, 100)) )
      {
        M_CallWalk(i, md);
      }
      else
      {
        v5 = random(106, 10);
        M_StartDelay(i, v5 + 15 - 2 * v7->_mint);
      }
    }
    else if ( v7->_mVar1 != 13 && (v2 = 2 * v7->_mint + 20, v2 <= random(105, 100)) )
    {
      v3 = random(105, 10);
      M_StartDelay(i, v3 + 10 - 2 * v7->_mint);
    }
    else
    {
      M_StartAttack(i);
    }
    if ( v7->_mmode == MM_STAND )
      v7->_mAnimData = v7->MType->Anims[0].Frames[md];
  }
}

//----- (0040A05D) --------------------------------------------------------
void __fastcall MAI_Bat(int i)
{
  int v1; // eax
  int v2; // eax
  int v3; // eax
  MonsterStruct *v5; // [esp+10h] [ebp-20h]
  int midir; // [esp+14h] [ebp-1Ch]
  int x2; // [esp+18h] [ebp-18h]
  int v8; // [esp+1Ch] [ebp-14h]
  int y1; // [esp+20h] [ebp-10h]
  int v10; // [esp+24h] [ebp-Ch]
  int micaster; // [esp+28h] [ebp-8h]
  int v12; // [esp+2Ch] [ebp-4h]

  v5 = &monster[i];
  micaster = monster[i]._menemy;
  if ( monster[i]._mmode == MM_STAND && v5->_msquelch )
  {
    if ( v5->_mFlags & 0x10 )
    {
      v12 = v5->_mx - monster[micaster]._mfutx;
      v1 = v5->_my - monster[micaster]._mfuty;
    }
    else
    {
      v12 = v5->_mx - plr[micaster]._px;
      v1 = v5->_my - plr[micaster]._py;
    }
    v10 = v1;
    midir = GetDirection(v5->_mx, v5->_my, v5->_lastx, v5->_lasty);
    v5->_mdir = midir;
    v8 = random(107, 100);
    if ( v5->_mgoal == 2 )
    {
      if ( v5->_mgoalvar1 )
      {
        if ( random(108, 2) )
          M_CallWalk(i, left[midir]);
        else
          M_CallWalk(i, right[midir]);
        v5->_mgoal = 1;
      }
      else
      {
        M_CallWalk(i, opposite[midir]);
        ++v5->_mgoalvar1;
      }
    }
    else
    {
      if ( v5->_mFlags & 0x10 )
      {
        y1 = monster[micaster]._mfutx;
        x2 = monster[micaster]._mfuty;
      }
      else
      {
        y1 = plr[micaster]._px;
        x2 = plr[micaster]._py;
      }
      if ( v5->MType->mtype == 40
        && (abs(v12) >= 5 || abs(v10) >= 5)
        && 4 * v5->_mint + 33 > v8
        && LineClearF1(i, v5->_mx, v5->_my, y1, x2) )
      {
        AddMissile(v5->_mx, v5->_my, y1, x2, midir, 20, micaster, i, 0);
        dMonster[v5->_mx][v5->_my] = -(i + 1);
        v5->_mmode = 14;
      }
      else if ( abs(v12) >= 2 || abs(v10) >= 2 )
      {
        if ( v5->_mVar2 > 20 && v5->_mint + 13 > v8
          || (v5->_mVar1 == 1 || v5->_mVar1 == 2 || v5->_mVar1 == 3) && !v5->_mVar2 && v5->_mint + 63 > v8 )
        {
          M_CallWalk(i, midir);
        }
      }
      else if ( 4 * v5->_mint + 8 > v8 )
      {
        M_StartAttack(i);
        v5->_mgoal = 2;
        v5->_mgoalvar1 = 0;
        if ( v5->MType->mtype == 41 )
        {
          if ( v5->_mFlags & 0x10 )
          {
            v3 = random(109, 10);
            AddMissile(
              monster[micaster]._mfutx,
              monster[micaster]._mfuty,
              monster[micaster]._mfutx + 1,
              0,
              -1,
              8,
              1,
              i,
              v3 + 1);
          }
          else
          {
            v2 = random(109, 10);
            AddMissile(plr[micaster]._px, plr[micaster]._py, plr[micaster]._px + 1, 0, -1, 8, 1, i, v2 + 1);
          }
        }
      }
      if ( v5->_mmode == MM_STAND )
        v5->_mAnimData = v5->MType->Anims[0].Frames[midir];
    }
  }
}

//----- (0040A583) --------------------------------------------------------
void __fastcall MAI_SkelBow(int i)
{
  int v1; // eax
  int v2; // edx
  int v3; // ebx
  MonsterStruct *v5; // [esp+10h] [ebp-24h]
  int v6; // [esp+14h] [ebp-20h]
  BOOL v7; // [esp+18h] [ebp-1Ch]
  int y2; // [esp+1Ch] [ebp-18h]
  int x2; // [esp+20h] [ebp-14h]
  int v10; // [esp+24h] [ebp-10h]
  int v11; // [esp+28h] [ebp-Ch]
  int enemy_; // [esp+2Ch] [ebp-8h]
  int v13; // [esp+30h] [ebp-4h]

  v7 = 0;
  v5 = &monster[i];
  if ( monster[i]._mmode == MM_STAND && v5->_msquelch )
  {
    enemy_ = v5->_menemy;
    if ( v5->_mFlags & 0x10 )
    {
      v13 = v5->_mx - monster[enemy_]._mfutx;
      v1 = v5->_my - monster[enemy_]._mfuty;
    }
    else
    {
      v13 = v5->_mx - plr[enemy_]._px;
      v1 = v5->_my - plr[enemy_]._py;
    }
    v11 = v1;
    v6 = M_GetDir(i, enemy_);
    v5->_mdir = v6;
    v10 = random(110, 100);
    if ( abs(v13) < 4
      && abs(v11) < 4
      && (v5->_mVar2 > 20 && 2 * v5->_mint + 13 > v10
       || (v5->_mVar1 == 1 || v5->_mVar1 == 2 || v5->_mVar1 == 3) && !v5->_mVar2 && 2 * v5->_mint + 63 > v10) )
    {
      v2 = opposite[v6];
      v7 = M_PathWalk(i);
    }
    if ( v5->_mFlags & 0x10 )
    {
      x2 = monster[enemy_]._mfutx;
      y2 = monster[enemy_]._mfuty;
    }
    else
    {
      x2 = plr[enemy_]._px;
      y2 = plr[enemy_]._py;
    }
    if ( !v7 )
    {
      v3 = 2 * v5->_mint + 3;
      if ( v3 > random(110, 100) )
      {
        if ( LineClear(v5->_mx, v5->_my, x2, y2) )
          M_StartRAttack(i, 0);
      }
    }
    if ( v5->_mmode == MM_STAND )
      v5->_mAnimData = v5->MType->Anims[0].Frames[v6];
  }
}

//----- (0040A863) --------------------------------------------------------
void __fastcall MAI_Fat(int i)
{
  int v1; // eax
  int md; // [esp+10h] [ebp-14h]
  int v4; // [esp+14h] [ebp-10h]
  int v5; // [esp+18h] [ebp-Ch]
  int plr; // [esp+1Ch] [ebp-8h]
  int v7; // [esp+20h] [ebp-4h]

  plr = monster[i]._menemy;
  if ( monster[i]._mmode == MM_STAND && monster[i]._msquelch )
  {
    if ( monster[i]._mFlags & 0x10 )
    {
      v7 = monster[i]._mx - monster[plr]._mfutx;
      v1 = monster[i]._my - monster[plr]._mfuty;
    }
    else
    {
      v7 = monster[i]._mx - ::plr[plr]._px;
      v1 = monster[i]._my - ::plr[plr]._py;
    }
    v5 = v1;
    md = M_GetDir(i, plr);
    monster[i]._mdir = md;
    v4 = random(111, 100);
    if ( abs(v7) >= 2 || abs(v5) >= 2 )
    {
      if ( monster[i]._mVar2 > 20 && 4 * monster[i]._mint + 20 > v4
        || (monster[i]._mVar1 == 1 || monster[i]._mVar1 == 2 || monster[i]._mVar1 == 3)
        && !monster[i]._mVar2
        && 4 * monster[i]._mint + 70 > v4 )
      {
        M_CallWalk(i, md);
      }
    }
    else if ( 4 * monster[i]._mint + 15 <= v4 )
    {
      if ( 4 * monster[i]._mint + 20 > v4 )
        M_StartSpAttack(i);
    }
    else
    {
      M_StartAttack(i);
    }
    if ( monster[i]._mmode == MM_STAND )
      monster[i]._mAnimData = monster[i].MType->Anims[0].Frames[md];
  }
}

//----- (0040ABA2) --------------------------------------------------------
void __fastcall MAI_Sneak(int i)
{
  int v1; // eax
  unsigned __int8 v2; // [esp+0h] [ebp-2Ch]
  MonsterStruct *m; // [esp+10h] [ebp-1Ch]
  int md; // [esp+14h] [ebp-18h]
  int v6; // [esp+18h] [ebp-14h]
  int v7; // [esp+1Ch] [ebp-10h]
  int v8; // [esp+20h] [ebp-Ch]
  int v9; // [esp+20h] [ebp-Ch]
  int enemy; // [esp+24h] [ebp-8h]
  int v11; // [esp+28h] [ebp-4h]
  int v12; // [esp+28h] [ebp-4h]

  m = &monster[i];
  if ( monster[i]._mmode == MM_STAND )
  {
    v11 = m->_mx;
    v8 = m->_my;
    if ( dTransVal[v11][v8] != lightmax )
    {
      enemy = m->_menemy;
      if ( m->_mFlags & 0x10 )
      {
        v12 = v11 - monster[enemy]._mfutx;
        v1 = monster[enemy]._mfuty;
      }
      else
      {
        v12 = v11 - plr[enemy]._px;
        v1 = plr[enemy]._py;
      }
      v9 = v8 - v1;
      md = M_GetDir(i, enemy);
      v7 = 5 - m->_mint;
      if ( m->_mVar1 == 5 )
      {
        m->_mgoal = 2;
        m->_mgoalvar1 = 0;
      }
      else if ( abs(v12) >= v7 + 3 || abs(v9) >= v7 + 3 || m->_mgoalvar1 > 8 )
      {
        m->_mgoal = 1;
        m->_mgoalvar1 = 0;
      }
      if ( m->_mgoal == 2 )
      {
        if ( m->_mFlags & 0x10 )
          md = GetDirection(m->_mx, m->_my, plr[m->_menemy]._pownerx, plr[m->_menemy]._pownery);
        md = opposite[md];
        if ( m->MType->mtype == 31 )
        {
          if ( random(112, 2) )
            md = left[md];
          else
            md = right[md];
        }
      }
      m->_mdir = md;
      v6 = random(112, 100);
      if ( abs(v12) >= v7 || abs(v9) >= v7 || m->_mFlags & 1 )
      {
        if ( (abs(v12) >= v7 + 1 || abs(v9) >= v7 + 1) && m->_mFlags & 1 )
        {
          M_StartFadeout(i, md, v2);
        }
        else if ( m->_mgoal == 2
               || (abs(v12) >= 2 || abs(v9) >= 2)
               && (m->_mVar2 > 20 && 4 * m->_mint + 14 > v6
                || (m->_mVar1 == 1 || m->_mVar1 == 2 || m->_mVar1 == 3) && !m->_mVar2 && 4 * m->_mint + 64 > v6) )
        {
          ++m->_mgoalvar1;
          M_CallWalk(i, md);
        }
      }
      else
      {
        M_StartFadein(i, md);
      }
      if ( m->_mmode == MM_STAND )
      {
        if ( abs(v12) >= 2 || abs(v9) >= 2 || 4 * m->_mint + 10 <= v6 )
          m->_mAnimData = m->MType->Anims[0].Frames[md];
        else
          M_StartAttack(i);
      }
    }
  }
}
// 40ABD6: user specified stroff has not been processed: MonsterStruct offset 28

//----- (0040B010) --------------------------------------------------------
void __fastcall MAI_Fireman(int i)
{
  int v1; // eax
  int v2; // eax
  unsigned __int8 v3; // [esp+0h] [ebp-30h]
  MonsterStruct *v5; // [esp+10h] [ebp-20h]
  int midir; // [esp+14h] [ebp-1Ch]
  int y2; // [esp+18h] [ebp-18h]
  int x2; // [esp+20h] [ebp-10h]
  int v9; // [esp+24h] [ebp-Ch]
  int micaster; // [esp+28h] [ebp-8h]
  int micastera; // [esp+28h] [ebp-8h]
  int v12; // [esp+2Ch] [ebp-4h]

  v5 = &monster[i];
  if ( monster[i]._mmode == MM_STAND && v5->_msquelch )
  {
    if ( v5->_mFlags & 0x10 )
    {
      x2 = monster[micaster]._mfutx;
      y2 = monster[micaster]._mfuty;
    }
    else
    {
      x2 = plr[micaster]._px;
      y2 = plr[micaster]._py;
    }
    micastera = v5->_menemy;
    if ( v5->_mFlags & 0x10 )
    {
      v12 = v5->_mx - monster[micastera]._mfutx;
      v1 = v5->_my - monster[micastera]._mfuty;
    }
    else
    {
      v12 = v5->_mx - plr[micastera]._px;
      v1 = v5->_my - plr[micastera]._py;
    }
    v9 = v1;
    midir = M_GetDir(i, micastera);
    switch ( v5->_mgoal )
    {
      case 1:
        if ( LineClear(v5->_mx, v5->_my, x2, y2) )
        {
          AddMissile(v5->_mx, v5->_my, x2, y2, midir, 50, micastera, i, 0);
          v5->_mmode = 14;
          v5->_mgoal = 6;
          v5->_mgoalvar1 = 0;
        }
        break;
      case 6:
        if ( v5->_mgoalvar1 == 3 )
        {
          v5->_mgoal = 1;
          M_StartFadeout(i, midir, v3);
        }
        else
        {
          if ( LineClear(v5->_mx, v5->_my, x2, y2) )
          {
            M_StartRAttack(i, 51);
          }
          else
          {
            v2 = random(112, 10);
            M_StartDelay(i, v2 + 5);
          }
          ++v5->_mgoalvar1;
        }
        break;
      case 2:
        M_StartFadein(i, midir);
        v5->_mgoal = 6;
        break;
    }
    v5->_mdir = midir;
    random(112, 100);
    if ( v5->_mmode == MM_STAND )
    {
      if ( abs(v12) >= 2 || abs(v9) >= 2 || v5->_mgoal != 1 )
      {
        if ( !M_CallWalk(i, midir) && (v5->_mgoal == 1 || v5->_mgoal == 2) )
        {
          M_StartFadein(i, midir);
          v5->_mgoal = 6;
        }
      }
      else
      {
        if ( monster[i]._mFlags & 0x10 )
          M_TryM2MHit(
            i,
            monster[i]._menemy,
            (unsigned __int8)monster[i].mHit,
            monster[i].mMinDamage,
            monster[i].mMaxDamage);
        else
          M_TryH2HHit(
            i,
            monster[i]._menemy,
            (unsigned __int8)monster[i].mHit,
            monster[i].mMinDamage,
            monster[i].mMaxDamage);
        v5->_mgoal = 2;
        if ( !M_CallWalk(i, opposite[midir]) )
        {
          M_StartFadein(i, midir);
          v5->_mgoal = 6;
        }
      }
    }
  }
}

//----- (0040B488) --------------------------------------------------------
void __fastcall MAI_Fallen(int i)
{
  int v1; // ST14_4
  int v2; // eax
  int v3; // eax
  MonsterStruct *v5; // [esp+14h] [ebp-2Ch]
  int v6; // [esp+18h] [ebp-28h]
  int v7; // [esp+18h] [ebp-28h]
  int j; // [esp+1Ch] [ebp-24h]
  int k; // [esp+20h] [ebp-20h]
  int v10; // [esp+24h] [ebp-1Ch]
  signed int v11; // [esp+28h] [ebp-18h]
  int plr; // [esp+30h] [ebp-10h]
  int v13; // [esp+34h] [ebp-Ch]
  int v14; // [esp+3Ch] [ebp-4h]

  v5 = &monster[i];
  if ( monster[i]._mgoal == 6 )
  {
    if ( v5->_mgoalvar1 )
      --v5->_mgoalvar1;
    else
      v5->_mgoal = 1;
  }
  if ( v5->_mmode == MM_STAND && v5->_msquelch )
  {
    if ( v5->_mgoal == 2 )
    {
      v1 = v5->_mgoalvar1--;
      if ( !v1 )
      {
        v5->_mgoal = 1;
        M_StartStand(i, opposite[v5->_mdir]);
      }
    }
    if ( v5->_mAnimLen == v5->_mAnimFrame )
    {
      if ( !random(113, 4) )
      {
        if ( !(monster[i]._mFlags & 8) )
        {
          M_StartSpStand(i, v5->_mdir);
          if ( v5->_mmaxhp - (2 * v5->_mint + 2) < v5->_mhitpoints )
            v5->_mhitpoints = v5->_mmaxhp;
          else
            v5->_mhitpoints += 2 * v5->_mint + 2;
        }
        v10 = 2 * v5->_mint + 4;
        for ( j = -v10; v10 >= j; ++j )
        {
          for ( k = -v10; v10 >= k; ++k )
          {
            if ( j >= 0 && j < 112 && k >= 0 && k < 112 )
            {
              v6 = dMonster[k + v5->_mx][j + v5->_my];
              if ( v6 > 0 )
              {
                if ( (v7 = v6 - 1, v11 = monster[v7].MType->mtype, v11 >= 4) && v11 <= 7 || v11 >= 12 && v11 <= 15 )
                {
                  monster[v7]._mgoal = 6;
                  monster[v7]._mgoalvar1 = 5 * (6 * v5->_mint + 21);
                }
              }
            }
          }
        }
      }
    }
    else if ( v5->_mgoal == 2 )
    {
      M_CallWalk(i, v5->_mdir);
    }
    else if ( v5->_mgoal == 6 )
    {
      plr = v5->_menemy;
      if ( v5->_mFlags & 0x10 )
      {
        v14 = v5->_mx - monster[plr]._mfutx;
        v2 = v5->_my - monster[plr]._mfuty;
      }
      else
      {
        v14 = v5->_mx - ::plr[plr]._px;
        v2 = v5->_my - ::plr[plr]._py;
      }
      v13 = v2;
      if ( abs(v14) >= 2 || abs(v13) >= 2 )
      {
        v3 = M_GetDir(i, plr);
        M_CallWalk(i, v3);
      }
      else
      {
        M_StartAttack(i);
      }
    }
    else
    {
      MAI_SkelSd(i);
    }
  }
}

//----- (0040B87F) --------------------------------------------------------
void __fastcall MAI_Cleaver(int i)
{
  int v1; // eax
  MonsterStruct *v3; // [esp+10h] [ebp-14h]
  int md; // [esp+14h] [ebp-10h]
  int v5; // [esp+18h] [ebp-Ch]
  int v6; // [esp+1Ch] [ebp-8h]
  int v7; // [esp+20h] [ebp-4h]

  v3 = &monster[i];
  v6 = monster[i]._menemy;
  if ( monster[i]._mmode == MM_STAND && v3->_msquelch )
  {
    if ( v3->_mFlags & 0x10 )
    {
      v7 = v3->_mx - monster[v6]._mfutx;
      v1 = v3->_my - monster[v6]._mfuty;
    }
    else
    {
      v7 = v3->_mx - plr[v6]._px;
      v1 = v3->_my - plr[v6]._py;
    }
    v5 = v1;
    md = GetDirection(v3->_mx, v3->_my, v3->_lastx, v3->_lasty);
    v3->_mdir = md;
    if ( abs(v7) >= 2 || abs(v5) >= 2 )
      M_CallWalk(i, md);
    else
      M_StartAttack(i);
    if ( v3->_mmode == MM_STAND )
      v3->_mAnimData = v3->MType->Anims[0].Frames[md];
  }
}

//----- (0040BA07) --------------------------------------------------------
void __fastcall MAI_Round(int i, unsigned __int8 special)
{
  int v2; // eax
  int v3; // ebx
  int v4; // ST1C_4
  int v5; // eax
  int special_; // [esp+Ch] [ebp-30h]
  MonsterStruct *v8; // [esp+18h] [ebp-24h]
  int mdir; // [esp+1Ch] [ebp-20h]
  int v10; // [esp+20h] [ebp-1Ch]
  int v11; // [esp+24h] [ebp-18h]
  int v12; // [esp+28h] [ebp-14h]
  int v13; // [esp+2Ch] [ebp-10h]
  int v14; // [esp+30h] [ebp-Ch]
  int v15; // [esp+34h] [ebp-8h]
  int v16; // [esp+38h] [ebp-4h]

  special_ = special;
  v8 = &monster[i];
  v15 = monster[i]._menemy;
  if ( monster[i]._mmode == MM_STAND && v8->_msquelch )
  {
    if ( v8->_mFlags & 0x10 )
    {
      v12 = monster[v15]._mfutx;
      v10 = monster[v15]._mfuty;
    }
    else
    {
      v12 = plr[v15]._px;
      v10 = plr[v15]._py;
    }
    if ( v8->_mFlags & 0x10 )
    {
      v16 = v8->_mx - monster[v15]._mfutx;
      v2 = v8->_my - monster[v15]._mfuty;
    }
    else
    {
      v16 = v8->_mx - plr[v15]._px;
      v2 = v8->_my - plr[v15]._py;
    }
    v14 = v2;
    mdir = GetDirection(v8->_mx, v8->_my, v8->_lastx, v8->_lasty);
    if ( (signed int)v8->_msquelch < 255 )
      MonstCheckDoors(i);
    v11 = random(114, 100);
    if ( (abs(v16) >= 2 || abs(v14) >= 2) && v8->_msquelch == 255 && dung_map[v8->_mx][v8->_my] == dung_map[v12][v10] )
    {
      if ( v8->_mgoal == 4 || (abs(v16) >= 4 || abs(v14) >= 4) && !random(115, 4) )
      {
        if ( v8->_mgoal != 4 )
        {
          v8->_mgoalvar1 = 0;
          v8->_mgoalvar2 = random(116, 2);
        }
        v8->_mgoal = 4;
        v3 = abs(v14);
        if ( v3 >= abs(v16) )
          v13 = abs(v14);
        else
          v13 = abs(v16);
        v4 = v8->_mgoalvar1++;
        if ( 2 * v13 <= v4 && DirOK(i, mdir) || dung_map[v8->_mx][v8->_my] != dung_map[v12][v10] )
        {
          v8->_mgoal = 1;
        }
        else if ( !M_RoundWalk(i, mdir, &v8->_mgoalvar2) )
        {
          v5 = random(125, 10);
          M_StartDelay(i, v5 + 10);
        }
      }
    }
    else
    {
      v8->_mgoal = 1;
    }
    if ( v8->_mgoal == 1 )
    {
      if ( abs(v16) >= 2 || abs(v14) >= 2 )
      {
        if ( v8->_mVar2 > 20 && 2 * v8->_mint + 28 > v11
          || (v8->_mVar1 == 1 || v8->_mVar1 == 2 || v8->_mVar1 == 3) && !v8->_mVar2 && 2 * v8->_mint + 78 > v11 )
        {
          M_CallWalk(i, mdir);
        }
      }
      else if ( 2 * v8->_mint + 23 > v11 )
      {
        v8->_mdir = mdir;
        if ( special_ && v8->_mmaxhp >> 1 > v8->_mhitpoints && random(117, 2) )
          M_StartSpAttack(i);
        else
          M_StartAttack(i);
      }
    }
    if ( v8->_mmode == MM_STAND )
      v8->_mAnimData = v8->MType->Anims[0].Frames[mdir];
  }
}

//----- (0040BF41) --------------------------------------------------------
void __fastcall MAI_GoatMc(int i)
{
  MAI_Round(i, 1u);
}

//----- (0040BF64) --------------------------------------------------------
void __fastcall MAI_Ranged(int i, int missile_type, unsigned __int8 special)
{
  int v3; // eax
  int v4; // ebx
  int v5; // ST20_4
  int v6; // [esp+0h] [ebp-34h]
  MonsterStruct *v9; // [esp+14h] [ebp-20h]
  int md; // [esp+18h] [ebp-1Ch]
  int y2; // [esp+20h] [ebp-14h]
  int x2; // [esp+24h] [ebp-10h]
  int v13; // [esp+28h] [ebp-Ch]
  int plr; // [esp+2Ch] [ebp-8h]
  int v15; // [esp+30h] [ebp-4h]

  v9 = &monster[i];
  if ( monster[i]._mmode == MM_STAND )
  {
    if ( v9->_msquelch == 255 )
    {
      plr = v9->_menemy;
      if ( v9->_mFlags & 0x10 )
      {
        v15 = v9->_mx - monster[plr]._mfutx;
        v13 = v9->_my - monster[plr]._mfuty;
        x2 = monster[plr]._mfutx;
        y2 = monster[plr]._mfuty;
      }
      else
      {
        v15 = v9->_mx - ::plr[plr]._px;
        v13 = v9->_my - ::plr[plr]._py;
        x2 = ::plr[plr]._px;
        y2 = ::plr[plr]._py;
      }
      md = M_GetDir(i, plr);
      if ( (signed int)v9->_msquelch < 255 )
        MonstCheckDoors(i);
      v9->_mdir = md;
      if ( v9->_mVar1 == 10 )
      {
        v3 = random(118, 20);
        M_StartDelay(i, v3);
      }
      else if ( abs(v15) < 4 && abs(v13) < 4 )
      {
        v4 = 10 * v9->_mint + 70;
        if ( v4 > random(119, 100) )
          M_CallWalk(i, opposite[md]);
      }
      if ( v9->_mmode == MM_STAND )
      {
        if ( LineClear(v9->_mx, v9->_my, x2, y2) )
        {
          if ( special )
            M_StartRSpAttack(i, missile_type, v6);
          else
            M_StartRAttack(i, missile_type);
        }
        else
        {
          v9->_mAnimData = v9->MType->Anims[0].Frames[md];
        }
      }
    }
    else if ( v9->_msquelch )
    {
      v5 = GetDirection(v9->_mx, v9->_my, v9->_lastx, v9->_lasty);
      M_CallWalk(i, v5);
    }
  }
}

//----- (0040C263) --------------------------------------------------------
void __fastcall MAI_GoatBow(int i)
{
  MAI_Ranged(i, 0, 0);
}

//----- (0040C285) --------------------------------------------------------
void __fastcall MAI_Succ(int i)
{
  MAI_Ranged(i, 24, 0);
}

//----- (0040C2AA) --------------------------------------------------------
void __fastcall MAI_AcidUniq(int i)
{
  MAI_Ranged(i, 57, 1u);
}

//----- (0040C2CF) --------------------------------------------------------
void __fastcall MAI_Scav(int i)
{
  int v1; // ecx
  MonsterStruct *v3; // [esp+10h] [ebp-10h]
  signed int y2; // [esp+14h] [ebp-Ch]
  int y2a; // [esp+14h] [ebp-Ch]
  signed int y2b; // [esp+14h] [ebp-Ch]
  signed int x2; // [esp+18h] [ebp-8h]
  int x2a; // [esp+18h] [ebp-8h]
  BOOL v9; // [esp+1Ch] [ebp-4h]

  v3 = &monster[i];
  v9 = 0;
  if ( monster[i]._mmode == MM_STAND )
  {
    if ( v3->_mmaxhp >> 1 > v3->_mhitpoints && v3->_mgoal != 3 )
    {
      if ( monster[i].leaderflag )
      {
        v1 = monster[i].leader;
        --monster[v1].packsize;
        monster[i].leaderflag = 0;
      }
      v3->_mgoal = 3;
      v3->_mgoalvar3 = 10;
    }
    if ( v3->_mgoal == 3 && v3->_mgoalvar3 )
    {
      --v3->_mgoalvar3;
      if ( dDead[v3->_mx][v3->_my] )
      {
        M_StartEat(i);
        if ( !(monster[i]._mFlags & 8) )
          v3->_mhitpoints += 64;
        if ( (v3->_mmaxhp >> 1) + (v3->_mmaxhp >> 2) <= v3->_mhitpoints )
        {
          v3->_mgoal = 1;
          v3->_mgoalvar1 = 0;
          v3->_mgoalvar2 = 0;
        }
      }
      else
      {
        if ( !v3->_mgoalvar1 )
        {
          if ( random(120, 2) )
          {
            for ( y2 = -4; y2 <= 4 && !v9; ++y2 )
            {
              for ( x2 = -4; x2 <= 4 && !v9; ++x2 )
              {
                if ( y2 >= 0 && y2 < 112 && x2 >= 0 && x2 < 112 )
                  v9 = dDead[x2 + v3->_mx][y2 + v3->_my]
                    && LineClearF(
                         (BOOL (__fastcall *)(int, int))CheckNoSolid,
                         v3->_mx,
                         v3->_my,
                         x2 + v3->_mx,
                         y2 + v3->_my);
              }
            }
            x2a = x2 - 1;
            y2a = y2 - 1;
          }
          else
          {
            for ( y2b = 4; y2b >= -4 && !v9; --y2b )
            {
              for ( x2 = 4; x2 >= -4 && !v9; --x2 )
              {
                if ( y2b >= 0 && y2b < 112 && x2 >= 0 && x2 < 112 )
                  v9 = dDead[x2 + v3->_mx][y2b + v3->_my]
                    && LineClearF(
                         (BOOL (__fastcall *)(int, int))CheckNoSolid,
                         v3->_mx,
                         v3->_my,
                         x2 + v3->_mx,
                         y2b + v3->_my);
              }
            }
            x2a = x2 + 1;
            y2a = y2b + 1;
          }
          if ( v9 )
          {
            v3->_mgoalvar1 = x2a + v3->_mx + 1;
            v3->_mgoalvar2 = y2a + v3->_my + 1;
          }
        }
        if ( v3->_mgoalvar1 )
        {
          v3->_mdir = GetDirection(v3->_mx, v3->_my, v3->_mgoalvar1 - 1, v3->_mgoalvar2 - 1);
          M_CallWalk(i, v3->_mdir);
        }
      }
    }
    if ( v3->_mmode == MM_STAND )
      MAI_SkelSd(i);
  }
}

//----- (0040C6FC) --------------------------------------------------------
void __fastcall MAI_Garg(int i)
{
  int v1; // ebx
  int v2; // ebx
  MonsterStruct *v4; // [esp+10h] [ebp-14h]
  int v5; // [esp+14h] [ebp-10h]
  int v6; // [esp+18h] [ebp-Ch]
  int v7; // [esp+20h] [ebp-4h]

  v4 = &monster[i];
  v7 = monster[i]._mx - monster[i]._lastx;
  v6 = monster[i]._my - monster[i]._lasty;
  v5 = M_GetDir(i, v4->_menemy);
  if ( v4->_mFlags & 4 )
  {
    if ( abs(v7) < v4->_mint + 2 )
    {
      v1 = v4->_mint + 2;
      if ( v1 > abs(v6) )
        v4->_mFlags &= 0xFFFFFFFB;
    }
  }
  else if ( v4->_mmode == MM_STAND && v4->_msquelch )
  {
    if ( v4->_mmaxhp >> 1 > v4->_mhitpoints )
      v4->_mgoal = 2;
    if ( v4->_mgoal == 2 )
    {
      if ( abs(v7) >= v4->_mint + 2 || (v2 = v4->_mint + 2, v2 <= abs(v6)) )
      {
        v4->_mgoal = 1;
        M_StartHeal(i);
      }
      else if ( !M_CallWalk(i, opposite[v5]) )
      {
        v4->_mgoal = 1;
      }
    }
    MAI_Round(i, 0);
  }
}

//----- (0040C89D) --------------------------------------------------------
void __fastcall MAI_RoundRanged(int i, int missile_type, unsigned __int8 checkdoors)
{
  int v3; // ebx
  int v4; // ST1C_4
  int v5; // eax
  int v6; // [esp+0h] [ebp-3Ch]
  MonsterStruct *v9; // [esp+18h] [ebp-24h]
  int mdir; // [esp+1Ch] [ebp-20h]
  int y2; // [esp+20h] [ebp-1Ch]
  int x2; // [esp+24h] [ebp-18h]
  int v13; // [esp+28h] [ebp-14h]
  int v14; // [esp+28h] [ebp-14h]
  int v15; // [esp+2Ch] [ebp-10h]
  int v16; // [esp+30h] [ebp-Ch]
  int v17; // [esp+34h] [ebp-8h]
  int v18; // [esp+38h] [ebp-4h]

  v9 = &monster[i];
  if ( monster[i]._mmode == MM_STAND && v9->_msquelch )
  {
    v17 = v9->_menemy;
    if ( v9->_mFlags & 0x10 )
    {
      v18 = v9->_mx - monster[v17]._mfutx;
      v16 = v9->_my - monster[v17]._mfuty;
      x2 = monster[v17]._mfutx;
      y2 = monster[v17]._mfuty;
    }
    else
    {
      v18 = v9->_mx - plr[v17]._px;
      v16 = v9->_my - plr[v17]._py;
      x2 = plr[v17]._px;
      y2 = plr[v17]._py;
    }
    mdir = GetDirection(v9->_mx, v9->_my, v9->_lastx, v9->_lasty);
    if ( checkdoors && (signed int)v9->_msquelch < 255 )
      MonstCheckDoors(i);
    v13 = random(121, 100);
    if ( (abs(v18) >= 2 || abs(v16) >= 2) && v9->_msquelch == 255 && dung_map[v9->_mx][v9->_my] == dung_map[x2][y2] )
    {
      if ( v9->_mgoal == 4 || (abs(v18) >= 3 || abs(v16) >= 3) && !random(122, 4) )
      {
        if ( v9->_mgoal != 4 )
        {
          v9->_mgoalvar1 = 0;
          v9->_mgoalvar2 = random(123, 2);
        }
        v9->_mgoal = 4;
        v3 = abs(v16);
        if ( v3 >= abs(v18) )
          v15 = abs(v16);
        else
          v15 = abs(v18);
        v4 = v9->_mgoalvar1++;
        if ( 2 * v15 <= v4 && DirOK(i, mdir) )
        {
          v9->_mgoal = 1;
        }
        else if ( 5 * v9->_mint + 5 > v13 && LineClear(v9->_mx, v9->_my, x2, y2) )
        {
          M_StartRSpAttack(i, missile_type, v6);
        }
        else
        {
          M_RoundWalk(i, mdir, &v9->_mgoalvar2);
        }
      }
    }
    else
    {
      v9->_mgoal = 1;
    }
    if ( v9->_mgoal == 1 )
    {
      if ( ((abs(v18) >= 3 || abs(v16) >= 3) && 5 * v9->_mint + 10 > v13 || 5 * v9->_mint + 5 > v13)
        && LineClear(v9->_mx, v9->_my, x2, y2) )
      {
        M_StartRSpAttack(i, missile_type, v6);
      }
      else if ( abs(v18) >= 2 || abs(v16) >= 2 )
      {
        v14 = random(124, 100);
        if ( 10 * v9->_mint + 50 > v14
          || (v9->_mVar1 == 1 || v9->_mVar1 == 2 || v9->_mVar1 == 3) && !v9->_mVar2 && 10 * v9->_mint + 80 > v14 )
        {
          M_CallWalk(i, mdir);
        }
      }
      else if ( 10 * v9->_mint + 60 > v13 )
      {
        v9->_mdir = mdir;
        M_StartAttack(i);
      }
    }
    if ( v9->_mmode == MM_STAND )
    {
      v5 = random(125, 10);
      M_StartDelay(i, v5 + 5);
    }
  }
}

//----- (0040CE03) --------------------------------------------------------
void __fastcall MAI_Magma(int i)
{
  MAI_RoundRanged(i, 21, 1u);
}

//----- (0040CE28) --------------------------------------------------------
void __fastcall MAI_Storm(int i)
{
  MAI_RoundRanged(i, 22, 1u);
}

//----- (0040CE4D) --------------------------------------------------------
void __fastcall MAI_Acid(int i)
{
  MAI_RoundRanged(i, 57, 0);
}

//----- (0040CE72) --------------------------------------------------------
void __fastcall MAI_Golum(int i)
{
  int v1; // ST2C_4
  MonsterStruct *v3; // [esp+10h] [ebp-18h]
  int md; // [esp+14h] [ebp-14h]
  int mda; // [esp+14h] [ebp-14h]
  BOOL v6; // [esp+18h] [ebp-10h]
  signed int j; // [esp+1Ch] [ebp-Ch]
  int v8; // [esp+20h] [ebp-8h]

  v3 = &monster[i];
  if ( !(monster[i]._mFlags & 0x10) )
    v3->_menemy = M_Enemy(i, 0);
  if ( v3->_mmode != 6 && v3->_mmode != 11 && (v3->_mmode < 1 || v3->_mmode > 3) && v3->_mmode != 4 )
  {
    v1 = v3->_mx - monster[v3->_menemy]._mfutx;
    v8 = v3->_my - monster[v3->_menemy]._mfuty;
    md = GetDirection(v3->_mx, v3->_my, v3->_lastx, v3->_lasty);
    v3->_mdir = md;
    if ( abs(v1) >= 2 || abs(v8) >= 2 )
    {
      v6 = M_CallWalk(i, md);
      if ( !v6 )
      {
        mda = ((unsigned __int8)M_GetDir(i, v3->_menemy) - 1) & 7;
        for ( j = 0; j < 8 && !v6; ++j )
        {
          mda = ((_BYTE)mda + 1) & 7;
          v6 = DirOK(i, mda);
        }
        M_WalkDir(i, mda);
      }
    }
    else
    {
      M_StartAttack(i);
    }
  }
}

//----- (0040D03B) --------------------------------------------------------
void __fastcall MAI_SkelKing(int i)
{
  int v1; // ebx
  int v2; // ST18_4
  int v3; // eax
  int v4; // eax
  MonsterStruct *v6; // [esp+14h] [ebp-30h]
  int mdir; // [esp+18h] [ebp-2Ch]
  int y2; // [esp+1Ch] [ebp-28h]
  int y; // [esp+20h] [ebp-24h]
  int x2; // [esp+24h] [ebp-20h]
  int v11; // [esp+28h] [ebp-1Ch]
  int v12; // [esp+28h] [ebp-1Ch]
  int x; // [esp+2Ch] [ebp-18h]
  int v14; // [esp+30h] [ebp-14h]
  int v15; // [esp+38h] [ebp-Ch]
  int v16; // [esp+3Ch] [ebp-8h]
  int v17; // [esp+40h] [ebp-4h]

  v6 = &monster[i];
  if ( monster[i]._mmode == MM_STAND && v6->_msquelch )
  {
    v16 = v6->_menemy;
    if ( v6->_mFlags & 0x10 )
    {
      v17 = v6->_mx - monster[v16]._mfutx;
      v15 = v6->_my - monster[v16]._mfuty;
      x2 = monster[v16]._mfutx;
      y2 = monster[v16]._mfuty;
    }
    else
    {
      v17 = v6->_mx - plr[v16]._px;
      v15 = v6->_my - plr[v16]._py;
      x2 = plr[v16]._px;
      y2 = plr[v16]._py;
    }
    mdir = GetDirection(v6->_mx, v6->_my, v6->_lastx, v6->_lasty);
    if ( (signed int)v6->_msquelch < 255 )
      MonstCheckDoors(i);
    v11 = random(126, 100);
    if ( (abs(v17) >= 2 || abs(v15) >= 2) && v6->_msquelch == 255 && dung_map[v6->_mx][v6->_my] == dung_map[x2][y2] )
    {
      if ( v6->_mgoal == 4 || (abs(v17) >= 3 || abs(v15) >= 3) && !random(127, 4) )
      {
        if ( v6->_mgoal != 4 )
        {
          v6->_mgoalvar1 = 0;
          v6->_mgoalvar2 = random(128, 2);
        }
        v6->_mgoal = 4;
        v1 = abs(v17);
        if ( v1 <= abs(v15) )
          v14 = abs(v15);
        else
          v14 = abs(v17);
        v2 = v6->_mgoalvar1++;
        if ( 2 * v14 <= v2 && DirOK(i, mdir) || dung_map[v6->_mx][v6->_my] != dung_map[x2][y2] )
        {
          v6->_mgoal = 1;
        }
        else if ( !M_RoundWalk(i, mdir, &v6->_mgoalvar2) )
        {
          v3 = random(125, 10);
          M_StartDelay(i, v3 + 10);
        }
      }
    }
    else
    {
      v6->_mgoal = 1;
    }
    if ( v6->_mgoal == 1 )
    {
      if ( gbMaxPlayers == 1
        && ((abs(v17) >= 3 || abs(v15) >= 3) && 4 * v6->_mint + 35 > v11 || v11 < 6)
        && LineClear(v6->_mx, v6->_my, x2, y2) )
      {
        x = v6->_mx + offset_x[mdir];
        y = v6->_my + offset_y[mdir];
        if ( PosOkMonst(i, x, y) && nummonsters < 200 )
        {
          M_SpawnSkel(x, y, mdir);
          M_StartSpStand(i, mdir);
        }
      }
      else if ( abs(v17) >= 2 || abs(v15) >= 2 )
      {
        v12 = random(129, 100);
        if ( v6->_mint + 25 <= v12
          && (v6->_mVar1 != 1 && v6->_mVar1 != 2 && v6->_mVar1 != 3 || v6->_mVar2 || v6->_mint + 75 <= v12) )
        {
          v4 = random(130, 10);
          M_StartDelay(i, v4 + 10);
        }
        else
        {
          M_CallWalk(i, mdir);
        }
      }
      else if ( v6->_mint + 20 > v11 )
      {
        v6->_mdir = mdir;
        M_StartAttack(i);
      }
    }
    if ( v6->_mmode == MM_STAND )
      v6->_mAnimData = v6->MType->Anims[0].Frames[mdir];
  }
}

//----- (0040D616) --------------------------------------------------------
void __fastcall MAI_Rhino(int i)
{
  int v1; // ST50_4
  int v2; // ST48_4
  int v3; // ebx
  int v4; // ST2C_4
  int v5; // eax
  int v6; // eax
  int m; // [esp+Ch] [ebp-2Ch]
  MonsterStruct *v8; // [esp+14h] [ebp-24h]
  int md; // [esp+18h] [ebp-20h]
  int x2; // [esp+1Ch] [ebp-1Ch]
  int y1; // [esp+20h] [ebp-18h]
  int v12; // [esp+24h] [ebp-14h]
  int v13; // [esp+24h] [ebp-14h]
  int v14; // [esp+28h] [ebp-10h]
  int v15; // [esp+2Ch] [ebp-Ch]
  int micaster; // [esp+30h] [ebp-8h]
  int v17; // [esp+34h] [ebp-4h]

  m = i;
  v8 = &monster[i];
  if ( monster[i]._mmode == MM_STAND )
  {
    micaster = v8->_menemy;
    v1 = v8->_mx;
    v2 = v8->_my;
    if ( v8->_msquelch )
    {
      if ( v8->_mFlags & 0x10 )
      {
        v17 = v8->_mx - monster[micaster]._mfutx;
        v15 = v8->_my - monster[micaster]._mfuty;
        y1 = monster[micaster]._mfutx;
        x2 = monster[micaster]._mfuty;
      }
      else
      {
        v17 = v8->_mx - plr[micaster]._px;
        v15 = v8->_my - plr[micaster]._py;
        y1 = plr[micaster]._px;
        x2 = plr[micaster]._py;
      }
      md = GetDirection(v8->_mx, v8->_my, v8->_lastx, v8->_lasty);
      if ( (signed int)v8->_msquelch < 255 )
        MonstCheckDoors(m);
      v12 = random(131, 100);
      if ( abs(v17) >= 2 || abs(v15) >= 2 )
      {
        if ( v8->_mgoal == 4 || (abs(v17) >= 5 || abs(v15) >= 5) && !random(132, 4) )
        {
          if ( v8->_mgoal != 4 )
          {
            v8->_mgoalvar1 = 0;
            v8->_mgoalvar2 = random(133, 2);
          }
          v8->_mgoal = 4;
          v3 = abs(v15);
          if ( v3 >= abs(v17) )
            v14 = abs(v15);
          else
            v14 = abs(v17);
          v4 = v8->_mgoalvar1++;
          if ( 2 * v14 > v4 && dung_map[v8->_mx][v8->_my] == dung_map[y1][x2] )
          {
            if ( !M_RoundWalk(m, md, &v8->_mgoalvar2) )
            {
              v5 = random(125, 10);
              M_StartDelay(m, v5 + 10);
            }
          }
          else
          {
            v8->_mgoal = 1;
          }
        }
      }
      else
      {
        v8->_mgoal = 1;
      }
      if ( v8->_mgoal == 1 )
      {
        if ( (abs(v17) >= 5 || abs(v15) >= 5) && 2 * v8->_mint + 43 > v12 && LineClearF1(m, v8->_mx, v8->_my, y1, x2) )
        {
          if ( v8->MData->snd_special )
            PlayEffect(m, 3);
          AddMissile(v8->_mx, v8->_my, y1, x2, md, 20, micaster, m, 0);
          dMonster[v8->_mx][v8->_my] = -(m + 1);
          v8->_mmode = 14;
        }
        else if ( abs(v17) >= 2 || abs(v15) >= 2 )
        {
          v13 = random(134, 100);
          if ( 2 * v8->_mint + 33 <= v13
            && (v8->_mVar1 != 1 && v8->_mVar1 != 2 && v8->_mVar1 != 3 || v8->_mVar2 || 2 * v8->_mint + 83 <= v13) )
          {
            v6 = random(135, 10);
            M_StartDelay(m, v6 + 10);
          }
          else
          {
            M_CallWalk(m, md);
          }
        }
        else if ( 2 * v8->_mint + 28 > v12 )
        {
          v8->_mdir = md;
          M_StartAttack(m);
        }
      }
      if ( v8->_mmode == MM_STAND )
        v8->_mAnimData = v8->MType->Anims[0].Frames[md];
    }
  }
}

//----- (0040DBA6) --------------------------------------------------------
void __fastcall MAI_Garbud(int i)
{
  int v1; // eax
  MonsterStruct *v3; // [esp+10h] [ebp-14h]
  int v4; // [esp+14h] [ebp-10h]
  int v5; // [esp+18h] [ebp-Ch]
  int v6; // [esp+18h] [ebp-Ch]
  int plr; // [esp+1Ch] [ebp-8h]
  int v8; // [esp+20h] [ebp-4h]
  int v9; // [esp+20h] [ebp-4h]

  v3 = &monster[i];
  if ( monster[i]._mmode == MM_STAND )
  {
    plr = v3->_menemy;
    v8 = v3->_mx;
    v5 = v3->_my;
    v4 = M_GetDir(i, v3->_menemy);
    if ( v3->mtalkmsg < 22 && v3->mtalkmsg > 18 && !(dFlags[v8][v5] & 2) && v3->_mgoal == 8 )
    {
      ++v3->mtalkmsg;
      v3->_mgoal = 7;
    }
    if ( dFlags[v8][v5] & 2 )
    {
      if ( v3->_mFlags & 0x10 )
      {
        v9 = v3->_mx - monster[plr]._mfutx;
        v1 = v3->_my - monster[plr]._mfuty;
      }
      else
      {
        v9 = v3->_mx - ::plr[plr]._px;
        v1 = v3->_my - ::plr[plr]._py;
      }
      v6 = v1;
      if ( (abs(v9) >= 2 || abs(v6) >= 2) && v3->_mgoal != 7 && qtextflag == 1 )
        qtextflag = 0;
      if ( v3->mtalkmsg == 22 && !qtextflag && v3->_mgoal == 8 )
      {
        v3->_mgoal = 1;
        v3->_msquelch = -1;
        v3->mtalkmsg = 0;
      }
    }
    if ( v3->_mgoal == 1 || v3->_mgoal == 4 )
      MAI_Round(i, 1u);
    if ( v3->_mmode == MM_STAND )
      v3->_mAnimData = v3->MType->Anims[0].Frames[v4];
  }
}

//----- (0040DE16) --------------------------------------------------------
void __fastcall MAI_SnotSpil(int i)
{
  int v1; // eax
  MonsterStruct *v3; // [esp+10h] [ebp-14h]
  int v4; // [esp+14h] [ebp-10h]
  int v5; // [esp+18h] [ebp-Ch]
  int v6; // [esp+18h] [ebp-Ch]
  int plr; // [esp+1Ch] [ebp-8h]
  int v8; // [esp+20h] [ebp-4h]
  int v9; // [esp+20h] [ebp-4h]

  v3 = &monster[i];
  if ( monster[i]._mmode == MM_STAND )
  {
    plr = v3->_menemy;
    v8 = v3->_mx;
    v5 = v3->_my;
    v4 = M_GetDir(i, v3->_menemy);
    if ( v3->mtalkmsg == 25 && !(dFlags[v8][v5] & 2) && v3->_mgoal == 8 )
    {
      ++v3->mtalkmsg;
      v3->_mgoal = 7;
    }
    if ( dFlags[v8][v5] & 2 )
    {
      if ( v3->_mFlags & 0x10 )
      {
        v9 = v3->_mx - monster[plr]._mfutx;
        v1 = v3->_my - monster[plr]._mfuty;
      }
      else
      {
        v9 = v3->_mx - ::plr[plr]._px;
        v1 = v3->_my - ::plr[plr]._py;
      }
      v6 = v1;
      if ( (abs(v9) >= 2 || abs(v6) >= 2) && v3->_mgoal != 7 && qtextflag == 1 )
        qtextflag = 0;
      if ( v3->mtalkmsg == 26 && !qtextflag && v3->_mgoal == 8 )
      {
        v3->_mgoal = 1;
        v3->_msquelch = -1;
        v3->mtalkmsg = 0;
      }
    }
    if ( v3->_mgoal == 1 )
      MAI_SkelSd(i);
    if ( v3->_mmode == MM_STAND )
      v3->_mAnimData = v3->MType->Anims[0].Frames[v4];
  }
}

//----- (0040E060) --------------------------------------------------------
void __cdecl DeleteMonsterList()
{
  int i; // [esp+Ch] [ebp-8h]

  i = 0;
  while ( nummonsters > i )
  {
    if ( monster[monstactive[i]]._mDelFlag )
    {
      DeleteMonster(i);
      i = 0;
    }
    else
    {
      ++i;
    }
  }
}

//----- (0040E0CD) --------------------------------------------------------
void __cdecl ProcessMonsters()
{
  int v0; // [esp+10h] [ebp-14h]
  int j; // [esp+14h] [ebp-10h]
  int i; // [esp+18h] [ebp-Ch]
  int v3; // [esp+1Ch] [ebp-8h]
  int v4; // [esp+20h] [ebp-4h]

  DeleteMonsterList();
  for ( j = 0; j < nummonsters; ++j )
  {
    i = monstactive[j];
    v0 = 0;
    if ( (signed int)gbMaxPlayers > 1 )
    {
      SetRndSeed(monster[i]._mAISeed);
      monster[i]._mAISeed = GetRndSeed();
    }
    if ( !(monster[j]._mFlags & 8)
      && monster[i]._mhitpoints < monster[i]._mmaxhp
      && (signed int)(monster[i]._mhitpoints & 0xFFFFFFC0) > 0 )
    {
      monster[i]._mhitpoints += monster[i].mLevel;
    }
    v4 = monster[i]._mx;
    v3 = monster[i]._my;
    if ( dFlags[v4][v3] & 2 && !monster[i]._msquelch && monster[i].MType->mtype == 51 )
      PlaySFX(224);
    if ( monster[i]._mFlags & 0x10 )
    {
      monster[i]._lastx = monster[monster[i]._menemy]._mfutx;
      monster[i]._lasty = monster[monster[i]._menemy]._mfuty;
    }
    else if ( dFlags[v4][v3] & 2 )
    {
      monster[i]._msquelch = -1;
      monster[i]._lastx = plr[monster[i]._menemy]._px;
      monster[i]._lasty = plr[monster[i]._menemy]._py;
    }
    else if ( monster[i]._msquelch )
    {
      --monster[i]._msquelch;
    }
    do
    {
      AiProc[monster[i].mAi](i);
      switch ( monster[i]._mmode )
      {
        case MM_STAND:
          v0 = M_DoStand(i);
          break;
        case MM_WALK:
          v0 = M_DoWalk(i);
          break;
        case MM_WALK2:
          v0 = M_DoWalk2(i);
          break;
        case MM_WALK3:
          v0 = M_DoWalk3(i);
          break;
        case MM_ATTACK:
          v0 = M_DoAttack(i);
          break;
        case MM_GOTHIT:
          v0 = M_DoGotHit(i);
          break;
        case MM_DEATH:
          v0 = M_DoDeath(i);
          break;
        case MM_SATTACK:
          v0 = M_DoSAttack(i);
          break;
        case MM_FADEIN:
          v0 = M_DoFadein(i);
          break;
        case MM_FADEOUT:
          v0 = M_DoFadeout(i);
          break;
        case MM_RATTACK:
          v0 = M_DoRAttack(i);
          break;
        case MM_SPSTAND:
          v0 = M_DoSpStand(i);
          break;
        case MM_RSPATTACK:
          v0 = M_DoRSpAttack(i);
          break;
        case MM_DELAY:
          v0 = M_DoDelay(i);
          break;
        case MM_CHARGE:
          v0 = 0;
          break;
        case MM_STONE:
          v0 = M_DoStone(i);
          break;
        case MM_HEAL:
          v0 = M_DoHeal(i);
          break;
        case MM_TALK:
          v0 = M_DoTalk(i);
          break;
        default:
          break;
      }
      if ( v0 )
        GroupUnity(i);
    }
    while ( v0 );
    if ( monster[i]._mmode != 15 && monster[i]._mAnimDelay <= ++monster[i]._mAnimCnt )
    {
      monster[i]._mAnimCnt = 0;
      if ( monster[i]._mFlags & 2 )
      {
        if ( !--monster[i]._mAnimFrame )
          monster[i]._mAnimFrame = monster[i]._mAnimLen;
      }
      else if ( !(monster[i]._mFlags & 4) && monster[i]._mAnimLen < ++monster[i]._mAnimFrame )
      {
        monster[i]._mAnimFrame = 1;
      }
    }
  }
  DeleteMonsterList();
}

//----- (0040E747) --------------------------------------------------------
void __cdecl FreeMonsters()
{
  int v0; // [esp+Ch] [ebp-Ch]
  signed int j; // [esp+10h] [ebp-8h]
  int i; // [esp+14h] [ebp-4h]

  for ( i = 0; i < nummtypes; ++i )
  {
    v0 = Monsters[i].mtype;
    for ( j = 0; j < 6; ++j )
    {
      if ( animletter[j] != 's' || monsterdata[v0].has_special )
      {
        mem_free_dbg(Monsters[i].Anims[j].CMem, 3968, "C:\\Diablo\\Direct\\MONSTER.CPP");
        Monsters[i].Anims[j].CMem = 0;
      }
    }
  }
  FreeMissiles2();
}

//----- (0040E83B) --------------------------------------------------------
BOOL __fastcall DirOK(int i, int mdir)
{
  int k; // [esp+14h] [ebp-18h]
  int j; // [esp+18h] [ebp-14h]
  int v7; // [esp+1Ch] [ebp-10h]
  int y; // [esp+20h] [ebp-Ch]
  int x; // [esp+24h] [ebp-8h]
  int v10; // [esp+28h] [ebp-4h]

  x = offset_x[mdir] + monster[i]._mx;
  y = offset_y[mdir] + monster[i]._my;
  if ( y < 0 || y >= 112 || x < 0 || x >= 112 )
    return 0;
  if ( !PosOkMonst(i, x, y) )
    return 0;
  switch ( mdir )
  {
    case 6:
      if ( SolidLoc(x, y + 1) )
        return 0;
      if ( dFlags[x][y + 1] & 0x10 )
        return 0;
      break;
    case 2:
      if ( SolidLoc(x + 1, y) )
        return 0;
      if ( dFlags[x + 1][y] & 0x10 )
        return 0;
      break;
    case 4:
      if ( SolidLoc(x + 1, y) )
        return 0;
      if ( SolidLoc(x, y + 1) )
        return 0;
      break;
    case 0:
      if ( SolidLoc(x - 1, y) )
        return 0;
      if ( SolidLoc(x, y - 1) )
        return 0;
      break;
  }
  if ( monster[i].leaderflag == 1 )
    return abs(x - monster[monster[i].leader]._mfutx) < 4 && abs(y - monster[monster[i].leader]._mfuty) < 4;
  if ( !monster[i]._uniqtype || !(UniqMonst[monster[i]._uniqtype - 1].mUnqAttr & 2) )
    return 1;
  v7 = 0;
  for ( j = x - 3; x + 3 >= j; ++j )
  {
    for ( k = y - 3; y + 3 >= k; ++k )
    {
      if ( k >= 0 && k < 112 && j >= 0 && j < 112 )
      {
        v10 = dMonster[j][k];
        if ( v10 < 0 )
          v10 = -v10;
        if ( v10 )
          --v10;
        if ( v10 < 0 )
          assertion_failed(4029, "C:\\Diablo\\Direct\\MONSTER.CPP");
        if ( monster[v10].leaderflag == 1
          && monster[v10].leader == i
          && monster[v10]._mfutx == j
          && monster[v10]._mfuty == k )
        {
          ++v7;
        }
      }
    }
  }
  return monster[i].packsize == v7;
}

//----- (0040EC81) --------------------------------------------------------
BOOL __fastcall PosOkMissile(int x, int y)
{
  return !nMissileTable[dPiece[x][y]] && !(dFlags[x][y] & 0x10);
}

//----- (0040ECED) --------------------------------------------------------
bool __fastcall CheckNoSolid(int x, int y)
{
  return nSolidTable[dPiece[x][y]] == 0;
}

//----- (0040ED39) --------------------------------------------------------
BOOL __fastcall LineClearF(BOOL (__fastcall *Clear)(int, int), int x1, int y1, int x2, int y2)
{
  int v5; // ebx
  int v6; // ST30_4
  int v7; // ST30_4
  int v8; // ST30_4
  int v9; // ST30_4
  int v11; // [esp+Ch] [ebp-34h]
  int v13; // [esp+14h] [ebp-2Ch]
  int v14; // [esp+18h] [ebp-28h]
  signed int v15; // [esp+1Ch] [ebp-24h]
  BOOL j; // [esp+20h] [ebp-20h]
  BOOL i; // [esp+20h] [ebp-20h]
  int v18; // [esp+24h] [ebp-1Ch]
  int v19; // [esp+24h] [ebp-1Ch]
  int v20; // [esp+28h] [ebp-18h]
  int v21; // [esp+28h] [ebp-18h]
  int v22; // [esp+30h] [ebp-10h]
  int v23; // [esp+34h] [ebp-Ch]
  int v24; // [esp+38h] [ebp-8h]
  int v25; // [esp+38h] [ebp-8h]
  signed int v26; // [esp+3Ch] [ebp-4h]

  v11 = x1;
  v23 = x1;
  v22 = y1;
  v14 = x2 - x1;
  v13 = y2 - y1;
  v5 = abs(y2 - y1);
  if ( v5 >= abs(v14) )
  {
    if ( v13 < 0 )
    {
      v8 = y1;
      y1 = y2;
      y2 = v8;
      v9 = v11;
      v11 = x2;
      x2 = v9;
      v13 = -v13;
      v14 = -v14;
    }
    if ( v14 <= 0 )
    {
      v19 = v13 + 2 * v14;
      v21 = 2 * v14;
      v25 = 2 * (v14 + v13);
      v15 = -1;
    }
    else
    {
      v19 = 2 * v14 - v13;
      v21 = 2 * v14;
      v25 = 2 * (v14 - v13);
      v15 = 1;
    }
    for ( i = (y1 != v22 || v23 != v11) && !Clear(v11, y1);
          !i && (y2 != y1 || x2 != v11);
          i = (++y1 != v22 || v23 != v11) && !Clear(v11, y1) )
    {
      if ( v19 <= 0 == v15 < 0 )
      {
        v19 += v25;
        v11 += v15;
      }
      else
      {
        v19 += v21;
      }
    }
  }
  else
  {
    if ( v14 < 0 )
    {
      v6 = v11;
      v11 = x2;
      x2 = v6;
      v7 = y1;
      y1 = y2;
      y2 = v7;
      v14 = -v14;
      v13 = -v13;
    }
    if ( v13 <= 0 )
    {
      v18 = v14 + 2 * v13;
      v20 = 2 * v13;
      v24 = 2 * (v14 + v13);
      v26 = -1;
    }
    else
    {
      v18 = 2 * v13 - v14;
      v20 = 2 * v13;
      v24 = 2 * (v13 - v14);
      v26 = 1;
    }
    for ( j = (v23 != v11 || y1 != v22) && !Clear(v11, y1);
          !j && (x2 != v11 || y2 != y1);
          j = (v23 != ++v11 || y1 != v22) && !Clear(v11, y1) )
    {
      if ( v18 <= 0 == v26 < 0 )
      {
        v18 += v24;
        y1 += v26;
      }
      else
      {
        v18 += v20;
      }
    }
  }
  return x2 == v11 && y2 == y1;
}

//----- (0040F098) --------------------------------------------------------
BOOL __fastcall LineClear(int x1, int y1, int x2, int y2)
{
  return LineClearF(PosOkMissile, x1, y1, x2, y2);
}

//----- (0040F0CC) --------------------------------------------------------
BOOL __fastcall LineClearF1(int i, int x1, int y1, int x2, int y2)
{
  int v5; // ebx
  int v6; // ST30_4
  int v7; // ST30_4
  int v8; // ST30_4
  int v9; // ST30_4
  int x; // [esp+Ch] [ebp-34h]
  int v13; // [esp+14h] [ebp-2Ch]
  int v14; // [esp+18h] [ebp-28h]
  signed int v15; // [esp+1Ch] [ebp-24h]
  BOOL k; // [esp+20h] [ebp-20h]
  BOOL j; // [esp+20h] [ebp-20h]
  int v18; // [esp+24h] [ebp-1Ch]
  int v19; // [esp+24h] [ebp-1Ch]
  int v20; // [esp+28h] [ebp-18h]
  int v21; // [esp+28h] [ebp-18h]
  int v22; // [esp+30h] [ebp-10h]
  int v23; // [esp+34h] [ebp-Ch]
  int v24; // [esp+38h] [ebp-8h]
  int v25; // [esp+38h] [ebp-8h]
  signed int v26; // [esp+3Ch] [ebp-4h]

  x = x1;
  v23 = x1;
  v22 = y1;
  v14 = x2 - x1;
  v13 = y2 - y1;
  v5 = abs(y2 - y1);
  if ( v5 >= abs(v14) )
  {
    if ( v13 < 0 )
    {
      v8 = y1;
      y1 = y2;
      y2 = v8;
      v9 = x;
      x = x2;
      x2 = v9;
      v13 = -v13;
      v14 = -v14;
    }
    if ( v14 <= 0 )
    {
      v19 = v13 + 2 * v14;
      v21 = 2 * v14;
      v25 = 2 * (v14 + v13);
      v15 = -1;
    }
    else
    {
      v19 = 2 * v14 - v13;
      v21 = 2 * v14;
      v25 = 2 * (v14 - v13);
      v15 = 1;
    }
    for ( j = (y1 != v22 || x != v23) && !PosOkMonst(i, x, y1);
          !j && (y2 != y1 || x2 != x);
          j = (++y1 != v22 || x != v23) && !PosOkMonst(i, x, y1) )
    {
      if ( v15 < 0 == v19 <= 0 )
      {
        v19 += v25;
        x += v15;
      }
      else
      {
        v19 += v21;
      }
    }
  }
  else
  {
    if ( v14 < 0 )
    {
      v6 = x;
      x = x2;
      x2 = v6;
      v7 = y1;
      y1 = y2;
      y2 = v7;
      v14 = -v14;
      v13 = -v13;
    }
    if ( v13 <= 0 )
    {
      v18 = v14 + 2 * v13;
      v20 = 2 * v13;
      v24 = 2 * (v14 + v13);
      v26 = -1;
    }
    else
    {
      v18 = 2 * v13 - v14;
      v20 = 2 * v13;
      v24 = 2 * (v13 - v14);
      v26 = 1;
    }
    for ( k = (x != v23 || y1 != v22) && !PosOkMonst(i, x, y1);
          !k && (x2 != x || y2 != y1);
          k = (++x != v23 || y1 != v22) && !PosOkMonst(i, x, y1) )
    {
      if ( v26 < 0 == v18 <= 0 )
      {
        v18 += v24;
        y1 += v26;
      }
      else
      {
        v18 += v20;
      }
    }
  }
  return x2 == x && y2 == y1;
}

//----- (0040F443) --------------------------------------------------------
void __fastcall unused_sub_40F443(int a1)
{
  char *v1; // eax
  int v2; // [esp+14h] [ebp-4h]

  monster[a1].MType = &Monsters[monster[a1]._mMTidx];
  monster[a1].MData = Monsters[monster[a1]._mMTidx].MData;
  if ( monster[a1]._uniqtype )
    v1 = UniqMonst[monster[a1]._uniqtype - 1].mName;
  else
    v1 = monster[a1].MData->mName;
  monster[a1].mName = v1;
  v2 = monster[a1]._mdir;
  switch ( monster[a1]._mmode )
  {
    case 0:
      monster[a1]._mAnimData = monster[a1].MType->Anims[0].Frames[v2];
      break;
    case 1:
      monster[a1]._mAnimData = monster[a1].MType->Anims[1].Frames[v2];
      break;
    case 2:
      monster[a1]._mAnimData = monster[a1].MType->Anims[1].Frames[v2];
      break;
    case 3:
      monster[a1]._mAnimData = monster[a1].MType->Anims[1].Frames[v2];
      break;
    case 4:
      monster[a1]._mAnimData = monster[a1].MType->Anims[2].Frames[v2];
      break;
    case 5:
      monster[a1]._mAnimData = monster[a1].MType->Anims[3].Frames[v2];
      break;
    case 6:
      monster[a1]._mAnimData = monster[a1].MType->Anims[4].Frames[v2];
      break;
    case 7:
      monster[a1]._mAnimData = monster[a1].MType->Anims[5].Frames[v2];
      break;
    case 8:
      monster[a1]._mAnimData = monster[a1].MType->Anims[5].Frames[v2];
      break;
    case 9:
      monster[a1]._mAnimData = monster[a1].MType->Anims[5].Frames[v2];
      break;
    case 0xA:
      monster[a1]._mAnimData = monster[a1].MType->Anims[2].Frames[v2];
      break;
    case 0xB:
      monster[a1]._mAnimData = monster[a1].MType->Anims[5].Frames[v2];
      break;
    case 0xC:
      monster[a1]._mAnimData = monster[a1].MType->Anims[5].Frames[v2];
      break;
    case 0xD:
      monster[a1]._mAnimData = monster[a1].MType->Anims[0].Frames[v2];
      break;
    case 0x10:
      monster[a1]._mAnimData = monster[a1].MType->Anims[5].Frames[v2];
      break;
    case 0x11:
      monster[a1]._mAnimData = monster[a1].MType->Anims[0].Frames[v2];
      break;
    default:
      return;
  }
}

//----- (0040F939) --------------------------------------------------------
void __fastcall M_FallenFear(int x, int y)
{
  int i; // [esp+18h] [ebp-Ch]
  int v5; // [esp+1Ch] [ebp-8h]
  int v6; // [esp+20h] [ebp-4h]

  for ( i = 0; nummonsters > i; ++i )
  {
    v6 = monstactive[i];
    v5 = 0;
    switch ( monster[v6].MType->mtype )
    {
      case 4u:
      case 0xCu:
        v5 = 7;
        break;
      case 5u:
      case 0xDu:
        v5 = 5;
        break;
      case 6u:
      case 0xEu:
        v5 = 3;
        break;
      case 7u:
      case 0xFu:
        v5 = 2;
        break;
      default:
        break;
    }
    if ( v5
      && abs(x - monster[v6]._mx) < 5
      && abs(y - monster[v6]._my) < 5
      && (signed int)(monster[v6]._mhitpoints & 0xFFFFFFC0) > 0 )
    {
      monster[v6]._mgoal = 2;
      monster[v6]._mgoalvar1 = v5;
      monster[v6]._mdir = GetDirection(x, y, monster[i]._mx, monster[i]._my);
    }
  }
}

//----- (0040FB1B) --------------------------------------------------------
void __fastcall PrintMonstHistory(int mt)
{
  sprintf(tempstr, "Total kills : %i", monstkills[mt]);
  AddPanelString(tempstr, 1);
  if ( monstkills[mt] > 40 )
  {
    sprintf(tempstr, "Hit Points : %i-%i", monsterdata[mt].mMinHP, monsterdata[mt].mMaxHP);
    AddPanelString(tempstr, 1);
  }
  if ( monstkills[mt] > 20 )
  {
    sprintf(
      tempstr,
      "Usual depth : %i",
      (15 * monsterdata[mt].mMinDLvl / 30 + 1 + 15 * monsterdata[mt].mMaxDLvl / 30 + 1) >> 1);
    AddPanelString(tempstr, 1);
  }
  pinfoflag = 1;
}

//----- (0040FC4C) --------------------------------------------------------
void __fastcall MissToMonst(int i, int x, int y)
{
  int v3; // ST3C_4
  int v4; // ST3C_4
  int py; // [esp+14h] [ebp-20h]
  int pya; // [esp+14h] [ebp-20h]
  MonsterStruct *v7; // [esp+18h] [ebp-1Ch]
  int px; // [esp+1Ch] [ebp-18h]
  int pxa; // [esp+1Ch] [ebp-18h]
  int ia; // [esp+20h] [ebp-14h]
  int v11; // [esp+28h] [ebp-Ch]
  int v12; // [esp+2Ch] [ebp-8h]

  ia = missile[i]._misource;
  v7 = &monster[ia];
  v12 = missile[i]._mix;
  v11 = missile[i]._miy;
  dMonster[x][y] = ia + 1;
  v7->_mdir = missile[i]._mimfnum;
  v7->_mx = x;
  v7->_my = y;
  v7->_mmode = 0;
  if ( v7->MType->mtype == 72 )
  {
    M_StartFadein(ia, v7->_mdir);
  }
  else if ( v7->_mFlags & 0x10 )
  {
    M2MStartHit(ia, -1, 0);
  }
  else
  {
    M_StartHit(ia, -1, 0);
  }
  if ( v7->_mFlags & 0x10 )
  {
    if ( dMonster[v12][v11] > 0 && v7->MType->mtype != 40 && v7->MType->mtype != 72 )
    {
      M_TryM2MHit(ia, dMonster[v12][v11] - 1, 100, v7->mMinDamage2, v7->mMaxDamage2);
      pxa = v12 + offset_x[v7->_mdir];
      pya = v11 + offset_y[v7->_mdir];
      if ( PosOkMonst(dMonster[v12][v11] - 1, v12 + offset_x[v7->_mdir], v11 + offset_y[v7->_mdir]) )
      {
        dMonster[pxa][pya] = dMonster[v12][v11];
        v4 = dMonster[pxa][pya];
        dMonster[v12][v11] = 0;
        monster[--v4]._mx = pxa;
        monster[v4]._mfutx = monster[v4]._mx;
        monster[v4]._my = pya;
        monster[v4]._mfuty = monster[v4]._my;
      }
    }
  }
  else if ( dPlayer[v12][v11] > 0 && v7->MType->mtype != 40 && v7->MType->mtype != 72 )
  {
    M_TryH2HHit(ia, dPlayer[v12][v11] - 1, 100, v7->mMinDamage2, v7->mMaxDamage2);
    px = v12 + offset_x[v7->_mdir];
    py = v11 + offset_y[v7->_mdir];
    if ( PosOkPlayer(dPlayer[v12][v11] - 1, v12 + offset_x[v7->_mdir], v11 + offset_y[v7->_mdir]) )
    {
      dPlayer[px][py] = dPlayer[v12][v11];
      v3 = dPlayer[px][py];
      dPlayer[v12][v11] = 0;
      plr[--v3].WorldX = px;
      plr[v3]._px = plr[v3].WorldX;
      plr[v3].WorldY = py;
      plr[v3]._py = plr[v3].WorldY;
    }
  }
}

//----- (00410116) --------------------------------------------------------
BOOL __fastcall PosOkMonst(int i, int x, int y)
{
  int v6; // [esp+14h] [ebp-10h]
  BOOL v7; // [esp+18h] [ebp-Ch]
  signed int v8; // [esp+1Ch] [ebp-8h]
  int v9; // [esp+20h] [ebp-4h]
  int j; // [esp+20h] [ebp-4h]

  v8 = 0;
  v7 = !SolidLoc(x, y) && !dPlayer[x][y] && !dMonster[x][y];
  if ( v7 && dObject[x][y] )
  {
    v6 = dObject[x][y] <= 0 ? -(dObject[x][y] + 1) : dObject[x][y] - 1;
    if ( object[v6]._oSolidFlag )
      v7 = 0;
  }
  if ( v7 && dMissile[x][y] && i >= 0 )
  {
    v9 = dMissile[x][y];
    if ( v9 > 0 )
    {
      if ( missile[v9]._mitype == 5 )
      {
        v8 = 1;
      }
      else
      {
        for ( j = 0; j < nummissiles; ++j )
        {
          if ( missile[missileactive[j]]._mitype == 5 )
            v8 = 1;
        }
      }
    }
    if ( v8 && !(monster[i].mMagicRes & 0x10) )
      v7 = 0;
  }
  return v7;
}

//----- (0041034B) --------------------------------------------------------
BOOL __fastcall IsSkel(int mt)
{
  return mt >= 8 && mt <= 11 || mt >= 20 && mt <= 23 || mt >= 24 && mt <= 27;
}

//----- (004103A9) --------------------------------------------------------
int __fastcall M_SpawnSkel(int x, int y, int dir)
{
  int v; // [esp+14h] [ebp-10h]
  int va; // [esp+14h] [ebp-10h]
  int v8; // [esp+18h] [ebp-Ch]
  int mtype; // [esp+1Ch] [ebp-8h]
  int mtypea; // [esp+1Ch] [ebp-8h]
  int i; // [esp+20h] [ebp-4h]

  v = 0;
  for ( mtype = 0; mtype < nummtypes; ++mtype )
  {
    if ( IsSkel(Monsters[mtype].mtype) )
      ++v;
  }
  if ( !v )
    return -1;
  v8 = random(136, v);
  va = 0;
  for ( mtypea = 0; mtypea < nummtypes && va <= v8; ++mtypea )
  {
    if ( IsSkel(Monsters[mtypea].mtype) )
      ++va;
  }
  i = AddMonster(x, y, dir, mtypea - 1, 1);
  if ( i != -1 )
    M_StartSpStand(i, dir);
  return i;
}

//----- (004104CA) --------------------------------------------------------
void __fastcall ActivateSpawn(int i, int x, int y, int dir)
{
  dMonster[x][y] = i + 1;
  monster[i]._mx = x;
  monster[i]._my = y;
  monster[i]._mfutx = x;
  monster[i]._mfuty = y;
  monster[i]._moldx = x;
  monster[i]._moldy = y;
  M_StartSpStand(i, dir);
}

//----- (00410595) --------------------------------------------------------
void __fastcall SpawnSkeleton(int ii, int x, int y)
{
  int v3; // ST04_4
  int v4; // ST1C_4
  int v5; // ST2C_4
  int v6; // ST04_4
  int x1; // [esp+14h] [ebp-3Ch]
  int x1a; // [esp+14h] [ebp-3Ch]
  int j; // [esp+18h] [ebp-38h]
  int k; // [esp+1Ch] [ebp-34h]
  int v13; // [esp+20h] [ebp-30h]
  int y1; // [esp+24h] [ebp-2Ch]
  int y1a; // [esp+24h] [ebp-2Ch]
  int v16[9]; // [esp+28h] [ebp-28h]
  int v17; // [esp+4Ch] [ebp-4h]

  if ( ii != -1 )
  {
    if ( PosOkMonst(-1, x, y) )
    {
      v3 = GetDirection(x, y, x, y);
      ActivateSpawn(ii, x, y, v3);
    }
    else
    {
      v13 = 0;
      y1 = 0;
      for ( j = y - 1; y + 1 >= j; ++j )
      {
        x1 = 0;
        for ( k = x - 1; x + 1 >= k; ++k )
        {
          v16[3 * x1 + y1] = PosOkMonst(-1, k, j);
          v13 |= v16[3 * x1++ + y1];
        }
        ++y1;
      }
      if ( v13 )
      {
        v17 = random(137, 15) + 1;
        x1a = 0;
        y1a = 0;
        while ( v17 > 0 )
        {
          if ( v16[3 * x1a + y1a] )
            --v17;
          if ( v17 > 0 && ++x1a == 3 )
          {
            x1a = 0;
            if ( ++y1a == 3 )
              y1a = 0;
          }
        }
        v4 = x - 1 + x1a;
        v5 = y - 1 + y1a;
        v6 = GetDirection(v4, v5, x, y);
        ActivateSpawn(ii, v4, v5, v6);
      }
    }
  }
}

//----- (0041075F) --------------------------------------------------------
int __cdecl PreSpawnSkeleton()
{
  int v; // [esp+Ch] [ebp-10h]
  int va; // [esp+Ch] [ebp-10h]
  int v3; // [esp+10h] [ebp-Ch]
  int mtype; // [esp+14h] [ebp-8h]
  int mtypea; // [esp+14h] [ebp-8h]
  int i; // [esp+18h] [ebp-4h]

  v = 0;
  for ( mtype = 0; mtype < nummtypes; ++mtype )
  {
    if ( IsSkel(Monsters[mtype].mtype) )
      ++v;
  }
  if ( !v )
    return -1;
  v3 = random(136, v);
  va = 0;
  for ( mtypea = 0; mtypea < nummtypes && va <= v3; ++mtypea )
  {
    if ( IsSkel(Monsters[mtypea].mtype) )
      ++va;
  }
  i = AddMonster(0, 0, 0, mtypea - 1, 0);
  if ( i != -1 )
    M_StartStand(i, 0);
  return i;
}

//----- (00410873) --------------------------------------------------------
void __fastcall TalktoMonster(int i)
{
  monster[i]._mmode = 17;
}

//----- (004108AB) --------------------------------------------------------
void __fastcall SpawnGolum(int i, int x, int y)
{
  int mtype; // [esp+18h] [ebp-8h]
  int ia; // [esp+1Ch] [ebp-4h]

  for ( mtype = 0; mtype < nummtypes && Monsters[mtype].mtype != 108; ++mtype )
    ;
  ia = AddMonster(i, x, 0, mtype, 1);
  if ( ia > 0 )
  {
    monster[ia]._mmaxhp = y << 6;
    monster[ia]._mhitpoints = y << 6;
    monster[ia]._menemy = M_Enemy(ia, 0);
    M_StartSpStand(ia, 0);
  }
}
