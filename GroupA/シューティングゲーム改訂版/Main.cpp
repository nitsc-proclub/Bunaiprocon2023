# include <Siv3D.hpp> // OpenSiv3D v0.6.5
#include "Bos1.hpp"
#include "EnemyH.hpp"
#include "EnemyN.hpp"
#include"Player.hpp"



struct Data//シーンで取り扱うデータ
{

	class data
	{
	public:
		String line;
		int high_score = 0;
		int score = 0;
		double time = 0;
		int life = 5;
		int kaisuu = 1;
		bool gameover = false;//trueの時にゲームオーバー画面に移動
		char gamelevel='N';
		double power=5;
		double spead=5;
		double bspead=5;
		size_t index0 = 0;
		int up = 100;
		bool clear=false;
		int gauge = 0;
	};

	class dataclear
	{
	public:

	};

	void dataretry()
	{
		me.PlayerClear();
		me.PBClear();
		enemyN.enemies_down.clear();
		enemyN.enemies_right.clear();
		enemyH.enemies_down.clear();
		enemyH.enemies_right.clear();
		data.kaisuu++;
		data.life = 5;
		data.time = 0;
		data.score = 0;
		bos1.Bos1.clear();

		if (data.index0 == 0)
		{
			bos1.Boslife = 1000;
		}
		else if (data.index0 == 1)
		{
			bos1.Boslife = 2000;
		}

	}

	void datafinish()
	{
		me.PlayerClear();
		me.PBClear();
		enemyN.enemies_down.clear();
		enemyN.enemies_right.clear();
		enemyH.enemies_down.clear();
		enemyH.enemies_right.clear();
		data.life = 5;
		data.time = 0;
		data.kaisuu = 1;
		data.score = 0;
		data.high_score = 0;
		bos1.Boslife = 1000;
		data.gamelevel = ' ';
		bos1.Bos1.clear();

	}

	void writtingscore()
	{
		TextReader SR(U"../../Score.txt");
		if (not SR)
		{
			throw Error{ U"SRのエラー" };
		}
		std::vector<int>memo = { 0,0,0,0,0 };
		String L;
		while (SR.readLine(L))
		{
			memo.push_back(Parse<int>(L));
		}
		memo.push_back(data.high_score);
		std::sort(memo.begin(), memo.end());
		std::reverse(memo.begin(), memo.end());
		for (int i = 0; i < memo.size()-7; i++)
		{
			memo.pop_back();
		}
		SR.close();
		TextWriter ScoreFile(U"../../Score.txt");
		if (not ScoreFile)
		{
			throw Error{ U"Failed to open `Score.txt(writer)`" };
		}
		ScoreFile.clear();
		for (int i = 0; i < memo.size(); i++)
		{
			ScoreFile << memo.at(i);
		}

	}

	data data;

	Player me;


	EnemyN enemyN;


	EnemyH enemyH;

	Bos1 bos1;

};


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

using App = SceneManager < String, Data >;

//タイトルシーン
class Title :public App::Scene {
public:
	Title(const InitData& init)
		:IScene{ init }
	{
		Scene::SetBackground(Palette::Skyblue);
	}
	void update() override
	{
		AudioAsset(U"titleM").play();

		SimpleGUI::RadioButtons(getData().data.index0, { U"ノーマル",U"ハード" }, Scene::CenterF().movedBy(-100, 50));
		if (getData().data.index0==0)
		{
			Scene::SetBackground(Palette::Skyblue);
			getData().bos1.Boslife = 1000;
		}
		if (getData().data.index0==1)
		{
			Scene::SetBackground(Palette::Red);
			getData().bos1.Boslife = 2000;
		}

		//自機カスタマイズのスライダー
		if (getData().data.index0==0)
		{
			if (SimpleGUI::Slider(U"パワー", getData().data.power, 1, 11, Vec2{ 410,340 }, 75, 150))
			{
				getData().data.spead += (15 - getData().data.power - getData().data.spead - getData().data.bspead) / 2; getData().data.bspead += (15 - getData().data.power - getData().data.spead - getData().data.bspead) / 2;
			}
			if (SimpleGUI::Slider(U"スピード", getData().data.spead, 0, 11, Vec2{ 410,370 }, 75, 150))
			{
				getData().data.power += (15 - getData().data.power - getData().data.spead - getData().data.bspead) / 2; getData().data.bspead += (15 - getData().data.power - getData().data.spead - getData().data.bspead) / 2;
			}
			if (SimpleGUI::Slider(U"弾の感覚", getData().data.bspead, 1, 11, Vec2{ 410,400 }, 75, 150))
			{
				getData().data.spead += (15 - getData().data.power - getData().data.spead - getData().data.bspead) / 2; getData().data.power += (15 - getData().data.power - getData().data.spead - getData().data.bspead) / 2;
			}
		}
		else if (getData().data.index0 == 1)
		{
			if (SimpleGUI::Slider(U"パワー", getData().data.power, 1, 11, Vec2{ 410,340 }, 75, 150))
			{
				getData().data.spead += (20 - getData().data.power - getData().data.spead - getData().data.bspead) / 2; getData().data.bspead += (20 - getData().data.power - getData().data.spead - getData().data.bspead) / 2;
			}
			if (SimpleGUI::Slider(U"スピード", getData().data.spead, 0, 11, Vec2{ 410,370 }, 75, 150))
			{
				getData().data.power += (20 - getData().data.power - getData().data.spead - getData().data.bspead) / 2; getData().data.bspead += (20 - getData().data.power - getData().data.spead - getData().data.bspead) / 2;
			}
			if (SimpleGUI::Slider(U"弾の感覚", getData().data.bspead, 1, 11, Vec2{ 410,400 }, 75, 150))
			{
				getData().data.spead += (20 - getData().data.power - getData().data.spead - getData().data.bspead) / 2; getData().data.power += (20 - getData().data.power - getData().data.spead - getData().data.bspead) / 2;
			}
		}

		if (KeyEnter.down())
		{
			AudioAsset(U"titleM").stop();
			if (getData().data.index0==0)
			{
				getData().data.gamelevel = 'N';
				changeScene(U"GameN");
			}
			else if (getData().data.index0 == 1) {
				getData().data.gamelevel = 'H';
				changeScene(U"GameH");
			}
		}
	}
	void draw() const override
	{
		RoundRect { 30, 300, 150, 170, 10 }.draw(Palette::White);
		TextReader ScoreReader(U"../../Score.txt");
		if (not ScoreReader)
		{
			throw Error{ U"Failed to open `Score.txt(reader)`" };
		}
		String l;
		Array<String>score;
		while (ScoreReader.readLine(l))
		{
			score.push_back(l);
		}
		FontAsset(U"scoreF")(U"1位  " + score[0] + U"点").draw(40, 310, Palette::Goldenrod);
		FontAsset(U"scoreF")(U"2位  " + score[1] + U"点").draw(40, 340, Palette::Gray);
		FontAsset(U"scoreF")(U"3位  " + score[2] + U"点").draw(40, 370, Palette::Saddlebrown);
		FontAsset(U"scoreF")(U"4位  " + score[3] + U"点").draw(40, 400, Palette::Black);
		FontAsset(U"scoreF")(U"5位  " + score[4] + U"点").draw(40, 430, Palette::Black);

		
		//自機カスタマイズ
		FontAsset(U"KaisuuF")(U"自機カスタマイズ").drawAt(510, 315, Palette::Black);

		FontAsset(U"TitleF")(U"ぴえんを倒せ！").drawAt(Scene::CenterF().movedBy(0, -60), Palette::Black);
		FontAsset(U"KaisuuF")(U"Enterを押して開始！").drawAt(Scene::CenterF().movedBy(0, 30), Palette::Black);
	}
};


//ゲームシーン ノーマルレベル
class GameN :public App::Scene
{
public:
	GameN(const InitData& init)
		:IScene{ init }
	{
		Scene::SetBackground(Palette::Skyblue);
	}
	void update() override
	{
		AudioAsset(U"BGMN").play();

		getData().me.MutekiTime += Scene::DeltaTime();
		getData().data.time += Scene::DeltaTime();
		getData().data.gameover = false;


		//下に移動する敵と自機の球の衝突判定
		for (auto itE = getData().enemyN.enemies_down.begin(); itE != getData().enemyN.enemies_down.end();)
		{
			const Circle EC{ *itE, 25/*ここの数字が「自機の弾vs敵」が実行される範囲（敵に判定がある）*/ }; //個々注意

			bool skip = false;

			for (auto itPB = getData().me.PBs.begin(); itPB != getData().me.PBs.end();)
			{
				if (EC.intersects(*itPB))
				{
					itE = getData().enemyN.enemies_down.erase(itE);
					getData().me.PBs.erase(itPB);
					skip = true;
					break;
				}
				itPB++;
			}
			if (skip) {
				getData().data.score++;
				continue;
			}
			itE++;
		}
		for (auto itE = getData().enemyN.enemies_down.begin(); itE != getData().enemyN.enemies_down.end();) {
			const Circle EC{ *itE, 25/*ここの数字が「自機の弾vs敵」が実行される範囲（敵に判定がある）*/ }; //個々注意
			bool skip = false;
			for (auto itPBu = getData().me.PBsu.begin(); itPBu != getData().me.PBsu.end();)
			{
				if (EC.intersects(*itPBu))
				{
					itE = getData().enemyN.enemies_down.erase(itE);
					getData().me.PBsu.erase(itPBu);
					skip = true;
					break;
				}
				itPBu++;
			}

			if (skip) {
				getData().data.score++;
				continue;
			}
			itE++;
		}
		for (auto itE = getData().enemyN.enemies_down.begin(); itE != getData().enemyN.enemies_down.end();) {
			const Circle EC{ *itE, 25/*ここの数字が「自機の弾vs敵」が実行される範囲（敵に判定がある）*/ }; //個々注意
			bool skip = false;
			for (auto itPBr = getData().me.PBsr.begin(); itPBr != getData().me.PBsr.end();)
			{
				if (EC.intersects(*itPBr))
				{
					itE = getData().enemyN.enemies_down.erase(itE);
					getData().me.PBsr.erase(itPBr);
					skip = true;
					break;
				}
				itPBr++;
			}
			if (skip) {
				getData().data.score++;
				continue;
			}
			itE++;
		}
		for (auto itE = getData().enemyN.enemies_down.begin(); itE != getData().enemyN.enemies_down.end();) {
			const Circle EC{ *itE, 25/*ここの数字が「自機の弾vs敵」が実行される範囲（敵に判定がある）*/ }; //個々注意
			bool skip = false;
			for (auto itPBl = getData().me.PBsl.begin(); itPBl != getData().me.PBsl.end();)
			{
				if (EC.intersects(*itPBl))
				{
					itE = getData().enemyN.enemies_down.erase(itE);
					getData().me.PBsl.erase(itPBl);
					skip = true;
					break;
				}
				itPBl++;
			}
			if (skip) {
				getData().data.score++;
				continue;
			}
			itE++;
		}



		//右に移動する敵と自機の球の衝突判定
		for (auto itE = getData().enemyN.enemies_right.begin(); itE != getData().enemyN.enemies_right.end();)
		{
			const Circle EC{ *itE, 25/*ここの数字が「自機の弾vs敵」が実行される範囲（敵に判定がある）*/ }; //個々注意
			bool skip = false;

			for (auto itPB = getData().me.PBs.begin(); itPB != getData().me.PBs.end();)
			{
				if (EC.intersects(*itPB))
				{
					itE = getData().enemyN.enemies_right.erase(itE);
					getData().me.PBs.erase(itPB);
					skip = true;
					break;
				}
				itPB++;
			}
			if (skip) {
				getData().data.score++;
				continue;
			}
			itE++;
		}
		for (auto itE = getData().enemyN.enemies_right.begin(); itE != getData().enemyN.enemies_right.end();)
		{
			const Circle EC{ *itE, 25/*ここの数字が「自機の弾vs敵」が実行される範囲（敵に判定がある）*/ }; //個々注意
			bool skip = false;
			for (auto itPB = getData().me.PBsu.begin(); itPB != getData().me.PBsu.end();)
			{
				if (EC.intersects(*itPB))
				{
					itE = getData().enemyN.enemies_right.erase(itE);
					getData().me.PBsu.erase(itPB);
					skip = true;
					break;
				}
				itPB++;
			}
			if (skip) {
				getData().data.score++;
				continue;
			}
			itE++;
		}
		for (auto itE = getData().enemyN.enemies_right.begin(); itE != getData().enemyN.enemies_right.end();)
		{
			const Circle EC{ *itE, 25/*ここの数字が「自機の弾vs敵」が実行される範囲（敵に判定がある）*/ }; //個々注意
			bool skip = false;
			for (auto itPB = getData().me.PBsr.begin(); itPB != getData().me.PBsr.end();)
			{
				if (EC.intersects(*itPB))
				{
					itE = getData().enemyN.enemies_right.erase(itE);
					getData().me.PBsr.erase(itPB);
					skip = true;
					break;
				}
				itPB++;
			}
			if (skip) {
				getData().data.score++;
				continue;
			}
			itE++;
		}
		for (auto itE = getData().enemyN.enemies_right.begin(); itE != getData().enemyN.enemies_right.end();)
		{
			const Circle EC{ *itE, 25/*ここの数字が「自機の弾vs敵」が実行される範囲（敵に判定がある）*/ }; //個々注意
			bool skip = false;
			for (auto itPB = getData().me.PBsl.begin(); itPB != getData().me.PBsl.end();)
			{
				if (EC.intersects(*itPB))
				{
					itE = getData().enemyN.enemies_right.erase(itE);
					getData().me.PBsl.erase(itPB);
					skip = true;
					break;
				}
				itPB++;
			}
			if (skip) {
				getData().data.score++;
				continue;
			}
			itE++;
		}



		for (const auto enemyPos : getData().enemyN.enemies_down)//down敵に当たった時
		{
			if (enemyPos.distanceFrom(getData().me.playerPos) <= 15 && getData().me.MutekiTime >= 2.0)
			{
				AudioAsset(U"hitM").play();
				getData().data.life--;
				getData().me.MutekiTime = 0;
			}
		}
		for (const auto enemyPos : getData().enemyN.enemies_right)//right敵に当たった時
		{
			if (enemyPos.distanceFrom(getData().me.playerPos) <= 15 && getData().me.MutekiTime >= 2.0)
			{
				AudioAsset(U"hitM").play();
				getData().data.life--;
				getData().me.MutekiTime = 0;
			}
		}


		if (getData().data.life <= 0)
		{
			getData().data.gameover = true;
		}
		if (getData().data.gameover == true)//ゲームオーバー時の処理
		{
			if (getData().data.score > getData().data.high_score)
			{
				getData().data.high_score = getData().data.score;
			}

			getData().data.score = 0;
			getData().bos1.timer = 0;
			AudioAsset(U"BGMN").stop();
			changeScene(U"GameOver", 1.5);
		}

		//ゲームクリアの処理
		if (getData().data.clear==true)
		{
			if (getData().data.score > getData().data.high_score)
			{
				getData().data.high_score = getData().data.score;
			}

			getData().data.score = 0;
			getData().bos1.timer = 0;
			AudioAsset(U"BGMN").stop();
			changeScene(U"Gameclear");
		}


		//自機の処理
		getData().me.PlayerMovement(getData().data.spead,getData().bos1.Bos1,getData().data.life,getData().me.MutekiTime);
		//自機の弾の処理
		getData().me.PBMovement(getData().data.bspead);
		//敵と敵の弾の処理
		getData().enemyN.EMovement(getData().data.score);
		//Bos1の処理
		getData().bos1.BosMove( getData().data.score,getData().me.playerPos, getData().me.PBs, getData().data.power, getData().data.index0,getData().data.clear,getData().data.gamelevel);

		//ボスの体力ゲージの計算
		if (getData().data.gamelevel=='N')
		{
			getData().data.gauge = 5;
		}
		else
		{
			getData().data.gauge = 10;
		}

	}
	void draw() const override
	{
		//操作範囲
		RoundRect{ 30,0,300,460,10 }(TextureAsset(U"back")).draw();
		//ボスの体力ゲージ(基礎)
		RoundRect{ 360,245,200,20,10 }.draw(Palette::White);
		//ボスの体力ゲージ
		RoundRect{ 360,245,getData().bos1.Boslife / getData().data.gauge,20,10 }.draw(Palette::Black);
		//スコア等の情報
		FontAsset(U"KaisuuF")(getData().data.kaisuu, U"回目").draw(565, 5);
		FontAsset(U"UIF")(U"Hscore").draw(360, 35);
		FontAsset(U"UIF")(U"{:0>6}"_fmt(getData().data.high_score)).draw(480, 35);
		FontAsset(U"UIF")(U"score").draw(360, 75);
		FontAsset(U"UIF")(U"{:0>4}"_fmt(getData().data.score)).draw(480, 75);
		FontAsset(U"UIF")(U"Time").draw(360, 115);
		FontAsset(U"UIF")(U"{:0>5.2f}"_fmt(getData().data.time)).draw(480, 115);
		FontAsset(U"UIF")(U"Life").draw(360, 155);
		FontAsset(U"UIF")(U"{:0>6}"_fmt(getData().data.life)).draw(480, 155);
		FontAsset(U"KaisuuF")(U"ぴえんのhp").draw(480, 205);
		//自機を描画
		getData().me.PlayerFigure();
		//自機の弾を描画
		getData().me.PBdraw();
		//敵を描画
		getData().enemyN.Edraw();
		//Bos1を描画
		getData().bos1.BosFigure(getData().data.score);
	}
};

//ゲームシーン　ハードレベル
class GameH :public App::Scene
{
public:

	GameH(const InitData& init)
		:IScene{ init }
	{
		Scene::SetBackground(Palette::Red);
	}
	void update() override
	{
		AudioAsset(U"BGMH").play();

		getData().me.MutekiTime += Scene::DeltaTime();
		getData().data.time += Scene::DeltaTime();
		getData().data.gameover = false;
		if (KeyQ.down())//開発者モード
		{
			getData().data.life = 0;
		}

		//自機の弾が敵に命中したとき
		for (auto itE = getData().enemyH.enemies_down.begin(); itE != getData().enemyH.enemies_down.end();)
		{
			const Circle EC{ *itE, 25/*ここの数字が「自機の弾vs敵」が実行される範囲（敵に判定がある）*/ }; //個々注意

			bool skip = false;

			for (auto itPB = getData().me.PBs.begin(); itPB != getData().me.PBs.end();)
			{
				if (EC.intersects(*itPB))
				{
					itE = getData().enemyH.enemies_down.erase(itE);
					getData().me.PBs.erase(itPB);
					skip = true;
					break;
				}
				itPB++;
			}
			if (skip) {
				getData().data.score++;
				continue;
			}
			itE++;
		}
		for (auto itE = getData().enemyH.enemies_down.begin(); itE != getData().enemyH.enemies_down.end();) {
			const Circle EC{ *itE, 25/*ここの数字が「自機の弾vs敵」が実行される範囲（敵に判定がある）*/ }; //個々注意
			bool skip = false;
			for (auto itPBu = getData().me.PBsu.begin(); itPBu != getData().me.PBsu.end();)
			{
				if (EC.intersects(*itPBu))
				{
					itE = getData().enemyH.enemies_down.erase(itE);
					getData().me.PBsu.erase(itPBu);
					skip = true;
					break;
				}
				itPBu++;
			}

			if (skip) {
				getData().data.score++;
				continue;
			}
			itE++;
		}
		for (auto itE = getData().enemyH.enemies_down.begin(); itE != getData().enemyH.enemies_down.end();) {
			const Circle EC{ *itE, 25/*ここの数字が「自機の弾vs敵」が実行される範囲（敵に判定がある）*/ }; //個々注意
			bool skip = false;
			for (auto itPBr = getData().me.PBsr.begin(); itPBr != getData().me.PBsr.end();)
			{
				if (EC.intersects(*itPBr))
				{
					itE = getData().enemyH.enemies_down.erase(itE);
					getData().me.PBsr.erase(itPBr);
					skip = true;
					break;
				}
				itPBr++;
			}
			if (skip) {
				getData().data.score++;
				continue;
			}
			itE++;
		}
		for (auto itE = getData().enemyH.enemies_down.begin(); itE != getData().enemyH.enemies_down.end();) {
			const Circle EC{ *itE, 25/*ここの数字が「自機の弾vs敵」が実行される範囲（敵に判定がある）*/ }; //個々注意
			bool skip = false;
			for (auto itPBl = getData().me.PBsl.begin(); itPBl != getData().me.PBsl.end();)
			{
				if (EC.intersects(*itPBl))
				{
					itE = getData().enemyH.enemies_down.erase(itE);
					getData().me.PBsl.erase(itPBl);
					skip = true;
					break;
				}
				itPBl++;
			}
			if (skip) {
				getData().data.score++;
				continue;
			}
			itE++;
		}


		for (auto itE = getData().enemyH.enemies_right.begin(); itE != getData().enemyH.enemies_right.end();)
		{
			const Circle EC{ *itE, 25/*ここの数字が「自機の弾vs敵」が実行される範囲（敵に判定がある）*/ }; //個々注意
			bool skip = false;

			for (auto itPB = getData().me.PBs.begin(); itPB != getData().me.PBs.end();)
			{
				if (EC.intersects(*itPB))
				{
					itE = getData().enemyH.enemies_right.erase(itE);
					getData().me.PBs.erase(itPB);
					skip = true;
					break;
				}
				itPB++;
			}
			if (skip) {
				getData().data.score++;
				continue;
			}
			itE++;
		}
		for (auto itE = getData().enemyH.enemies_right.begin(); itE != getData().enemyH.enemies_right.end();)
		{
			const Circle EC{ *itE, 25/*ここの数字が「自機の弾vs敵」が実行される範囲（敵に判定がある）*/ }; //個々注意
			bool skip = false;
			for (auto itPB = getData().me.PBsu.begin(); itPB != getData().me.PBsu.end();)
			{
				if (EC.intersects(*itPB))
				{
					itE = getData().enemyH.enemies_right.erase(itE);
					getData().me.PBsu.erase(itPB);
					skip = true;
					break;
				}
				itPB++;
			}
			if (skip) {
				getData().data.score++;
				continue;
			}
			itE++;
		}
		for (auto itE = getData().enemyH.enemies_right.begin(); itE != getData().enemyH.enemies_right.end();)
		{
			const Circle EC{ *itE, 25/*ここの数字が「自機の弾vs敵」が実行される範囲（敵に判定がある）*/ }; //個々注意
			bool skip = false;
			for (auto itPB = getData().me.PBsr.begin(); itPB != getData().me.PBsr.end();)
			{
				if (EC.intersects(*itPB))
				{
					itE = getData().enemyH.enemies_right.erase(itE);
					getData().me.PBsr.erase(itPB);
					skip = true;
					break;
				}
				itPB++;
			}
			if (skip) {
				getData().data.score++;
				continue;
			}
			itE++;
		}
		for (auto itE = getData().enemyH.enemies_right.begin(); itE != getData().enemyH.enemies_right.end();)
		{
			const Circle EC{ *itE, 25/*ここの数字が「自機の弾vs敵」が実行される範囲（敵に判定がある）*/ }; //個々注意
			bool skip = false;
			for (auto itPB = getData().me.PBsl.begin(); itPB != getData().me.PBsl.end();)
			{
				if (EC.intersects(*itPB))
				{
					itE = getData().enemyH.enemies_right.erase(itE);
					getData().me.PBsl.erase(itPB);
					skip = true;
					break;
				}
				itPB++;
			}
			if (skip) {
				getData().data.score++;
				continue;
			}
			itE++;
		}


		for (const auto enemyPos : getData().enemyH.enemies_down)//down敵に当たった時
		{
			if (enemyPos.distanceFrom(getData().me.playerPos) <= 15 && getData().me.MutekiTime >= 2.0)
			{
				AudioAsset(U"hitM").play();
				getData().data.life--;
				getData().me.MutekiTime = 0;
			}
		}
		for (const auto enemyPos : getData().enemyH.enemies_right)//right敵に当たった時
		{
			if (enemyPos.distanceFrom(getData().me.playerPos) <= 15 && getData().me.MutekiTime >= 2.0)
			{
				AudioAsset(U"hitM").play();
				getData().data.life--;
				getData().me.MutekiTime = 0;
			}
		}

		if (getData().data.life <= 0)
		{
			getData().data.gameover = true;
		}
		if (getData().data.gameover == true)//ゲームオーバー時の処理
		{
			if (getData().data.score > getData().data.high_score)
			{
				getData().data.high_score = getData().data.score;
			}
			getData().data.score = 0;
			getData().bos1.timer = 0;

			AudioAsset(U"BGMH").stop();
			changeScene(U"GameOver", 1.5);
		}

		//ゲームクリアの処理
		if (getData().data.clear == true)
		{
			if (getData().data.score > getData().data.high_score)
			{
				getData().data.high_score = getData().data.score;
			}

			getData().data.score = 0;
			getData().bos1.timer = 0;
			AudioAsset(U"BGMH").stop();
			changeScene(U"Gameclear");
		}

		//ボスの体力ゲージの計算
		if (getData().data.gamelevel == 'N')
		{
			getData().data.gauge = 5;
		}
		else
		{
			getData().data.gauge = 10;
		}

		//自機の処理
		getData().me.PlayerMovement(getData().data.spead,getData().bos1.Bos1,getData().data.life, getData().me.MutekiTime);
		//自機の弾の処理
		getData().me.PBMovement(getData().data.bspead);
		//敵と敵の弾の処理
		getData().enemyH.EMovement(getData().data.score);
		//Bos1の処理
		getData().bos1.BosMove(getData().data.score, getData().me.playerPos, getData().me.PBs, getData().data.power, getData().data.index0,getData().data.clear,getData().data.gamelevel);
	}
	void draw() const override
	{
		//操作範囲
		RoundRect{ 30,0,300,460,10 }(TextureAsset(U"back")).draw();
		//ボスの体力ゲージ(基礎)
		RoundRect{ 360,245,200,20,10 }.draw(Palette::White);
		//ボスの体力ゲージ
		RoundRect{ 360,245,getData().bos1.Boslife / getData().data.gauge,20,10 }.draw(Palette::Black);

		//スコア等の情報
		FontAsset(U"KaisuuF")(getData().data.kaisuu, U"回目").draw(565, 5);
		FontAsset(U"UIF")(U"Hscore").draw(360, 35);
		FontAsset(U"UIF")(U"{:0>6}"_fmt(getData().data.high_score)).draw(480, 35);
		FontAsset(U"UIF")(U"score").draw(360, 75);
		FontAsset(U"UIF")(U"{:0>5}"_fmt(getData().data.score)).draw(480, 75);
		FontAsset(U"UIF")(U"Time").draw(360, 115);
		FontAsset(U"UIF")(U"{:0>5.2f}"_fmt(getData().data.time)).draw(480, 115);
		FontAsset(U"UIF")(U"Life").draw(360, 155);
		FontAsset(U"UIF")(U"{:0>6}"_fmt(getData().data.life)).draw(480, 155);
		FontAsset(U"KaisuuF")(U"ぴえんのhp").draw(480, 205);
		//自機を描画
		getData().me.PlayerFigure();
		//自機の弾を描画
		getData().me.PBdraw();
		//敵を描画
		getData().enemyH.Edraw();
		//Bos1を描画
		getData().bos1.BosFigure(getData().data.score);
	}
};


//ゲームクリアシーン
class Gameclear :public App::Scene
{
public:
	Gameclear(const InitData& init)
		:IScene{ init }
	{
		if (getData().data.gamelevel == 'N')
		{
			Scene::SetBackground(Palette::Skyblue);
		}
		else if (getData().data.gamelevel == 'H')
		{
			Scene::SetBackground(Palette::Red);
		}
		AudioAsset(U"clearM").play();
	}


	String clearMess = U"CLEAR";
	double zoom = 1.0;
	String cleasMess = U"CLEAR";
	double timetime=0;
	void update() override
	{
		const size_t length = static_cast<size_t>(timetime += Scene::DeltaTime() / 0.2);
		FontAsset(U"TitleF")(clearMess.substr(0, length)).drawAt(Scene::CenterF().movedBy(0, -20),Palette::Black);
		FontAsset(U"KaisuuF")(getData().data.high_score).drawAt(Scene::CenterF().movedBy(0, 30),Palette::Navy);

		if (KeyEnter.down())
		{
			getData().writtingscore();
			getData().datafinish();
			AudioAsset(U"clearM").stop();
			changeScene(U"Title");
		}
	}

	void draw() const override
	{
		FontAsset(U"KaisuuF")(U"Enterでタイトルに戻る").drawAt(Scene::CenterF().movedBy(170,200));
	}
};



//ゲームオーバーシーン
class GameOver :public App::Scene
{
public:
	GameOver(const InitData& init)
		:IScene{ init }
	{
		if (getData().data.gamelevel=='N')
		{
			Scene::SetBackground(Palette::Skyblue);
		}
		else if (getData().data.gamelevel == 'H')
		{
			Scene::SetBackground(Palette::Red);
		}
		AudioAsset(U"retryM").play();
	}
	String text = U"Rを押してリスタート";
	double timetime = 0;

	void update() override
	{
		AudioAsset(U"gameoverM").play();

		const size_t length = static_cast<size_t>(timetime += Scene::DeltaTime() / 0.17);
		FontAsset(U"TitleF")(text.substr(0, length)).drawAt(Scene::Center().movedBy(0, 30), Palette::Black);
		FontAsset(U"KaisuuF")(getData().data.high_score).drawAt(Scene::CenterF().movedBy(0, 100),Palette::Navy);

		if (KeyR.down())
		{
			getData().dataretry();

			AudioAsset(U"retryM").stop();
			AudioAsset(U"gameoverM").stop();

			if (getData().data.gamelevel=='N')
			{
			changeScene(U"GameN");
			}
			else if (getData().data.gamelevel == 'H')
			{
				changeScene(U"GameH");
			}

		}
		if (KeyDelete.down())
		{

			getData().writtingscore();

			getData().datafinish();
			AudioAsset(U"retryM").stop();
			AudioAsset(U"gameoverM").stop();
			changeScene(U"Title");
		}
	}
	void draw() const override
	{
		FontAsset(U"TitleF")(U"GAMEOVER").drawAt(Scene::CenterF().movedBy(0, -30), Palette::Black);
		FontAsset(U"KaisuuF")(U"Deleteで終了").drawAt(Scene::CenterF().movedBy(0, 70), Palette::Black);

	}
};

void Main() {
	ResizeMode::Keep;
	Scene::Resize(640, 480);
	//Font
	FontAsset::Register(U"TitleF", 50, U"../../Font/Corporate-Logo-Bold-ver2.otf");
	FontAsset::Register(U"UIF", 25, U"../../Font/cameliab.ttf");
	FontAsset::Register(U"KaisuuF", 20, U"../../Font/CP Font.otf");
	FontAsset::Register(U"scoreF", 20, U"../../Font/Corporate-Logo-Bold-ver2.otf");
	//Audio
	AudioAsset::Register(U"titleM", U"../../GameMusic/title.mp3");
	AudioAsset::Register(U"BGMN", U"../../GameMusic/BGM.mp3");
	AudioAsset::Register(U"gameoverM", U"../../GameMusic/ゲームオーバー.mp3");
	AudioAsset::Register(U"hitM", U"../../GameMusic/my_damage.mp3");
	AudioAsset::Register(U"retryM", U"../../GameMusic/リトライ.wav");
	AudioAsset::Register(U"clearM", U"../../GameMusic/clear.mp3");
	AudioAsset::Register(U"deleteE", U"../../GameMusic/enemy_down.mp3");
	AudioAsset::Register(U"BGMH", U"../../GameMusic/BGMH.mp3");

	//Texture
	TextureAsset::Register(U"playerT", U"../../Pictures/player.png");
	TextureAsset::Register(U"enemyT", U"../../Pictures/teki.png");
	TextureAsset::Register(U"Bos1", U"../../Pictures/Bos1.png");
	TextureAsset::Register(U"back", U"../../Pictures/back.png");


	App manager;

	//タイトルシーン
	manager.add<Title>(U"Title");
	//ゲームシーン ノーマルレベル
	manager.add<GameN>(U"GameN");
	//ゲームシーン　ハードレベル
	manager.add<GameH>(U"GameH");
	//ゲームシーン　クリアシーン
	manager.add<Gameclear>(U"Gameclear");
	//ゲームオーバーシーン
	manager.add<GameOver>(U"GameOver");

	while (System::Update())
	{
		if (not manager.update())break;
	}

}


//score.txtには最低10行数字を記入してください
//音楽：魔王魂、イワシロ音楽素材

//やること
//上向きの球を対ボス用に変化させる。
//クリアシーン

