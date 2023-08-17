#pragma once
#include<Siv3D.hpp>
struct Player//自機クラス
{
	Vec2 playerPos = { 180,430 }; //自機のポジ
	double PBtime = 0;//タイマー
	double PBtime2 = 0;
	
	double PBSpeed = 6;//弾の速度
	double MutekiTime = 0;//無敵時間
	Array<Vec2> PBs;
	Array<Vec2> PBsu;
	Array<Vec2> PBsr;
	Array<Vec2> PBsl;
	const Texture happy{ U"../../Pictures/happyface.png" };


	//used in update()
	void PlayerMovement(double ps,Array<Circle> bos1,int& life,double& muteki) {
		//自機の移動
		if (KeyRight.pressed()) {
			playerPos.x += Scene::DeltaTime() * (200+13*ps);
		}
		else if (KeyD.pressed()) {
			playerPos.x += Scene::DeltaTime() * (200 + 13 * ps);
		}
		if (KeyLeft.pressed()) {
			playerPos.x -= Scene::DeltaTime() * (200 + 13 * ps);
		}
		else if (KeyA.pressed()) {
			playerPos.x -= Scene::DeltaTime() * (200 + 13 * ps);
		}
		if (KeyUp.pressed()) {
			playerPos.y -= Scene::DeltaTime() * (200 + 13 * ps);
		}
		else if (KeyW.pressed()) {
			playerPos.y -= Scene::DeltaTime() * (200 + 13 * ps);
		}
		if (KeyDown.pressed()) {
			playerPos.y += Scene::DeltaTime() * (200 + 13 * ps);
		}
		else if (KeyS.pressed()) {
			playerPos.y += Scene::DeltaTime() * (200 + 13 * ps);
		}

		//自機が画面外に出ないようにする
		if (playerPos.x > 320)//右
		{
			playerPos.x = 320;
		}
		if (playerPos.y > 450)//下
		{
			playerPos.y = 450;
		}
		if (playerPos.x < 40)//左
		{
			playerPos.x = 40;
		}
		if (playerPos.y < 10)//上
		{
			playerPos.y = 10;
		}

		for (auto &Bos1:bos1)//未完成
		{
			if (Bos1.intersects(playerPos)&&muteki>2)
			{
				life--;
				muteki = 0;
			}
		}

	}

	//used in draw()
	void PlayerFigure()
	{
		//自機を描画
		TextureAsset(U"playerT").scaled(0.1).drawAt(playerPos);
		
	}

	//used in update()
	void PBMovement(double bspead)
	{
		PBtime2 += Scene::DeltaTime();
		PBtime += Scene::DeltaTime();
		if (PBtime >= 0.5-0.035*bspead)
		{
			PBs << Vec2{ playerPos.x,playerPos.y -30};
			PBsu << Vec2{ playerPos.x,playerPos.y+30 };
			PBtime = 0;
		}
		if (PBtime2 >= (0.5 - 0.035 * bspead)+1)
		{
			PBsr << Vec2{ playerPos.x+20,playerPos.y };
			PBsl << Vec2{ playerPos.x-20,playerPos.y };
			PBtime2 = 0;
		}

		for (auto& PBS : PBs)
		{
			PBS.y -= PBSpeed;
		}
		for (auto& PBS : PBsu)
		{
			PBS.y += PBSpeed;
		}
		for (auto& PBS : PBsr)
		{
			PBS.x += PBSpeed;
		}
		for (auto& PBS : PBsl)
		{
			PBS.x -= PBSpeed;
		}

		PBs.remove_if([](const Vec2& v) {return(0 > v.y); });
		PBsu.remove_if([](const Vec2& v) {return( 450 < v.y); });
		PBsl.remove_if([](const Vec2& v) {return(30 > v.x); });
		PBsr.remove_if([](const Vec2& v) {return(300 < v.x); });

	}

	//used in draw()
	void PBdraw()
	{
		for (const auto& PBS : PBs)
		{
			Circle{ PBS,10 }(happy).draw(Palette::White);
		}
		for (const auto& PBS : PBsu)
		{
			Circle{ PBS,1.8 }.draw(Palette::White);
		}
		for (const auto& PBS : PBsr)
		{
			Circle{ PBS,1.8 }.draw(Palette::White);
		}
		for (const auto& PBS : PBsl)
		{
			Circle{ PBS,1.8 }.draw(Palette::White);
		}

	}

	void PlayerClear()//ゲームオーバー時の関数
	{
		playerPos.x = 180; playerPos.y = 430;
	}

	void PBClear()//ゲームオーバー時の関数
	{
		PBs.clear();
		PBsu.clear();
		PBsr.clear();
		PBsl.clear();

	}


};
