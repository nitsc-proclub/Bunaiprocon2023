#pragma once
#include "function.hpp"

struct EnemyH//ハードレベル敵クラス
{
	double EInterval = 0.3;//敵の出現間隔
	double etime = 0;
	Array<Vec2>enemies_down;//下移動の敵の保管配列
	Array<Vec2>enemies_right;//右移動の敵の保管配列

	void enemy1()
	{
		if (etime >= EInterval)
		{
			enemies_down << RandomVec2(RectF{30,20,300,1});
			etime = 0;
		}
	}
	void enemy2()
	{
		if (etime >= EInterval)
		{
			enemies_down << RandomVec2(RectF{ 30,20,300,1 });
			etime = 0;
		}
	}
	void enemy3()
	{
		if (probability(0.5) == true)
		{
			if (etime >= EInterval)
			{
				enemies_right << RandomVec2(RectF{ 30,20, 1,430});
				etime = 0;
			}
		}
		else
		{
			if (etime >= EInterval)
			{
				enemies_down << RandomVec2(RectF{ 30,20,300,1});
				etime = 0;
			}
		}
	}
	//used in update()
	void EMovement(int& score)
	{
		etime += Scene::DeltaTime();

		if (score < 25)
		{
			enemy1();
			for (auto& enemy : enemies_down)
			{
				enemy.y += Scene::DeltaTime() * 240;//毎秒240px
			}
		}
		else if (score >= 25 && score < 50)
		{
			enemy2();
			for (auto& enemy : enemies_down)
			{
				enemy.y += Scene::DeltaTime() * 420;//毎秒420px
			}

			if (etime >= EInterval)
			{
				enemies_down << RandomVec2(RectF{ 30,20,300,1});
				etime = 0;
			}
		}
		else if (score >= 50)
		{
			enemy3();
			for (auto& enemy : enemies_down)
			{
				enemy.y += Scene::DeltaTime() * 400;//毎秒400px
			}
			for (auto& enemy : enemies_right)
			{
				enemy.x += Scene::DeltaTime() * 500;//毎秒500px
			}

			if (etime >= EInterval)
			{
				enemies_right << RandomVec2(RectF{ 30,20,1,420 });
				etime = 0;
			}
		}

		enemies_down.remove_if([](const Vec2& v) {return(460 < v.y); });
		enemies_right.remove_if([](const Vec2& v) {return(330 < v.x); });
	}

	//used in draw()
	void Edraw()
	{
		for (const auto& enemy : enemies_down)
		{
			TextureAsset(U"enemyT").scaled(0.03).drawAt(enemy);
		}
		for (const auto& enemy : enemies_right)
		{
			TextureAsset(U"enemyT").scaled(0.03).drawAt(enemy);
		}
	}
};
