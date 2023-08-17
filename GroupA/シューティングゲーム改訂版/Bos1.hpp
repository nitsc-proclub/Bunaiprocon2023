#pragma once
#include <Siv3D.hpp>

struct Bos1
{
	double Boslife ;
	double Bosspeedx = 3;
	double Bosspeedy = 6;
	double BosX = 30;
	double BosY = 10;
	Array<Circle>Bos1;
	double timer=0;
	double TImer = 0;
	double a = Random();
	double b = Random();
	int count = 0;

	//used in update()
	void BosMove(double score, Vec2 PPos, Array<Vec2>PBs, double PBDamege, size_t& index0,bool& clear,char GL)
	{
		double Bx=0, By=0;
			if (a <= 0.25)
			{
				Bx = Periodic::Sine0_1(Bosspeedx);
			}
			else if (a > 0.33 && a <= 0.66)
			{
				Bx = Periodic::Jump0_1(Bosspeedx);
			}
			else 
			{
				Bx = Periodic::Triangle0_1(Bosspeedx);
			}

			if (b <= 0.33)
			{
				By = Periodic::Sine0_1(Bosspeedy);
			}
			else if (b > 0.33 && b <= 0.66)
			{
				By = Periodic::Jump0_1(Bosspeedy);
			}
			else 
			{
				By = Periodic::Triangle0_1(Bosspeedy);
			}


		if (score > 50)
		{
			timer += Scene::DeltaTime();
			if (Bos1.size() == 0)
			{
				Bos1.push_back(Circle{ 0,0,40 });
			}
			BosX = Bx * 230+60;
			BosY = By * 460;
			if (Bos1.empty()==false&&Boslife>=0)
			{
				for (auto& BOS1 : Bos1)
				{
					BOS1.y =BosY;
					BOS1.x = BosX;
				}
			}
			if (Bos1.empty()==false)
			{
				Bos1.remove_if([](const Circle& v) {return(460 < v.y); });
			}

			if (Boslife <= 0&&Bos1.empty() == false)
			{
				Bos1.pop_back();
				a = Random();
				score += 10 * (50/timer);
				timer = 0;
			}

			if (Boslife<=0&&Bos1.empty()==true)
			{
				TImer += Scene::DeltaTime();
				if (TImer>5)
				{
					if (GL=='N')
					{
						Bosspeedx -= 0.3;
						Bosspeedy -= 0.6;
						Boslife = 1000;
					}
					else
					{
						Bosspeedx -= 0.5;
						Bosspeedy -= 1.0;
						Boslife = 2000;
					}
					if (count==5)
					{
						clear = true;
					}
					Bos1.push_back(Circle{ 0,0,40 });
					count++;
					TImer = 0;
				}
			}

			if (Bos1.empty()==false)
			{
				for (auto PBS : PBs)
				{
					for (const auto& BOs1 : Bos1)
					{
						if (BOs1.intersects(PBS))
						{
							Boslife -= (5.0+PBDamege);
						}
					}
			

				}

			}
		}
	}

	//used in draw()
	void BosFigure(double score)
	{
		if (score > 50)
		{
			for (const auto& BoS1 : Bos1)
			{
				if (Boslife<=0)
				{
					break;
				}
				BoS1(TextureAsset(U"Bos1")).draw();
			}
		}
	}
};
