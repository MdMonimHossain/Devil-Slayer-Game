# include "iGraphics.h"
# include "string.h"
# include "stdlib.h"
# include "time.h"

#define SCRN_WIDTH 1000
#define SCRN_HEIGHT 600
#define SPEED 3
#define scoreLimit 5000

//for different menu showing
bool mainWindow = true, playWindow, hSWindow, hTPWindow, aboutUsWindow, gameOver;

//for highlighting icons
bool playHighlight, hSHighlight, hTPHighlight, aUHighlight, exitHighlight, backHighlight, playAgainHighlight, backtoMenuHighlight, yesHighlight, noHighlight;
bool menuUnmutedPop, menuMutedPop, unmutedPop, mutedPop, pausePop, resumePop, quitPop;

//for inputing player name
bool nameInput, inputMode;
char nameStr[20];
int nameLength;

//for quiting game
bool quitGameBox;

//for getting saved high scores
int totalHighScores;

//miscellineous
char scoreStr[100000];
int randomSeq[4];

//for player
char playerName[20];
int playerScore = 0;
int health = 225;

//for background movement
bool bgMovement;
int bg1_x, bg2_x = 990;

//for game mechanics
bool paused;
bool counting;
bool musicOn = true;
int countImageNo = 2;
int bgRenderingTimer, characterAnimationTimer, countTimer, spawnTimer, jumpAnimationTimer, attackAnimationTimer, scoreTimer;
char set;
int collision[8];

//images
int bgImage[6], buttonImage[10], iconImage[14], frameImage, countImage[3], healthbarImage, ScoreImage, noHighScores, quitImage, fireBallImage, lifeImage;
int idleImage[10], runImage[10], jumpImage[10], attackImage[10], dieImage[10];
int obsImage[2], batImage[4], zombieImage[5];
int bossImage, bossFireImage, winImage;

//character
int character_x = 300, character_y = 78;
bool hurt;
int characterShow;

//character idling
bool idling;
int idleImageNo;
//character running
bool running;
int runImageNo;
//character jumping
bool jumping;
int jumpImageNo;
int jumpY;
bool yFixed;
//character attacking
bool attacking;
int attackImageNo;
//character dying
bool dying;
int dieImageNo;
//character jump firing
bool jumpFiring;
int jumpFiringImageNo;

//enemy animation
int zombieAImageNo, zombieBImageNo;
int batAImageNo, batBImageNo;

//boss stage
bool bossStage;
int dy = 2;

void EnemyObstacleInitialize();

struct EnemyObstacle
{
	int x, y;
	int height, width;
	bool show;

}setA[4], setB[4];

struct FireBall
{
	int x, y;
	bool show;

}fireball[2], bossFire;

struct Life
{
	int x, y;
	int height, width;
	bool show;

}life;

struct EnemyBoss
{
	int x, y;
	int height, width;
	int health;
	bool show;
	bool defeated;
}boss;

struct NameAndScore
{
	char name[20];
	int score;

}nameScore[5];

void GetHighScores()
{
	FILE *fp = fopen("HighScores.txt","r");

	if(fp == NULL)
	{
		printf("file not opened");
	}
	else
	{
		while(fscanf(fp, "%s\t%d", nameScore[totalHighScores].name, &nameScore[totalHighScores].score) != EOF)
		{
		   totalHighScores++;
		}

		fclose(fp);
	}
}

void ShowHighScores()
{
	if(totalHighScores == 0)
	{
		iShowImage(0, 0, 860, 600, noHighScores);
	}
	if(totalHighScores >= 1)
	{
		iText(105, 390, nameScore[0].name, GLUT_BITMAP_TIMES_ROMAN_24);
		sprintf(scoreStr,"%d", nameScore[0].score);
		iText(760, 390, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	if(totalHighScores >= 2)
	{
		iText(105, 340, nameScore[1].name, GLUT_BITMAP_TIMES_ROMAN_24);
		sprintf(scoreStr,"%d", nameScore[1].score);
		iText(760, 340, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	if(totalHighScores >= 3)
	{
		iText(105, 290, nameScore[2].name, GLUT_BITMAP_TIMES_ROMAN_24);
		sprintf(scoreStr,"%d", nameScore[2].score);
		iText(760, 290, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	if(totalHighScores >= 4)
	{
		iText(105, 240, nameScore[3].name, GLUT_BITMAP_TIMES_ROMAN_24);
		sprintf(scoreStr,"%d", nameScore[3].score);
		iText(760, 240, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	if(totalHighScores == 5)
	{
		iText(105, 190, nameScore[4].name, GLUT_BITMAP_TIMES_ROMAN_24);
		sprintf(scoreStr,"%d", nameScore[4].score);
		iText(760, 190, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
	}
}

void SaveHighScore()
{
	int i, j;
	bool flag = false;

	if(totalHighScores==5)
    {
        for(i = 0; i < totalHighScores; i++)
        {
            if(nameScore[i].score < playerScore)
            {
                for(j = totalHighScores - 1; j > i; j--)
                {
                    nameScore[j].score = nameScore[j - 1].score;
                    strcpy(nameScore[j].name, nameScore[j - 1].name);
                }

                nameScore[i].score = playerScore;
                strcpy(nameScore[i].name, playerName);
                break;
            }
        }
    }
	else
    {
        for(i = 0; i < totalHighScores; i++)
        {
            if(nameScore[i].score < playerScore)
            {
                for(j = totalHighScores; j > i; j--)
                {
                    nameScore[j].score = nameScore[j - 1].score;
                    strcpy(nameScore[j].name, nameScore[j - 1].name);
                }

                nameScore[i].score = playerScore;
                strcpy(nameScore[i].name, playerName);
                flag = true;
                break;
            }
        }

        if(!flag)
        {
            nameScore[totalHighScores].score = playerScore;
            strcpy(nameScore[totalHighScores].name, playerName);
        }

        totalHighScores++;
    }

	FILE *fp = fopen("HighScores.txt","w");

    for(i = 0; i < totalHighScores; i++)
    {
        fprintf(fp,"%s\t%d\n",nameScore[i].name, nameScore[i].score);
    }

    fclose(fp);
}

void NameInputBox()
{
	iSetColor(255, 255, 255);
	iFilledRectangle(300, 200, 400, 250);

	iShowImage(300, 200, 400, 250, frameImage);

	iSetColor(50, 50, 50);
	iFilledRectangle(380, 300, 240, 50);
	iSetColor(0, 0, 0);

	iText(335, 390, "Enter Your Name : (Max 10 Characters)", GLUT_BITMAP_HELVETICA_18);

	if(!inputMode)
	{
		iText(335, 250, "Click on the box to start typing", GLUT_BITMAP_9_BY_15);
	}
	else
	{
		iSetColor(255, 255, 255);
		iText(400, 318, nameStr, GLUT_BITMAP_HELVETICA_18);
		iSetColor(0, 0, 0);
		iText(335, 240, "Press 'Enter' to Submit", GLUT_BITMAP_9_BY_15);
		iText(335, 260, "Press 'Backspace' to delete", GLUT_BITMAP_9_BY_15);
	}
}

void QuitGameBox()
{
	iShowImage(300, 200, 400, 250, quitImage);
	iShowImage(300, 200, 400, 250, frameImage);
	iShowImage(370, 250, 100, 40, buttonImage[8]);
	iShowImage(530, 250, 100, 40, buttonImage[9]);
	iSetColor(50, 50, 50);

	if(yesHighlight)
	{
		iRectangle(368, 248, 105, 45);
	}
	if(noHighlight)
	{
		iRectangle(528, 248, 105, 45);
	}
	
}

void StartCounting()
{
	if(counting)
	{
		if(countImageNo > 0)
		{
			countImageNo--;
		}
		else
		{
			countImageNo = 2;
			counting = false;
			bgMovement = true;
			idling = false;
			running = true;
			iPauseTimer(countTimer);
			iResumeTimer(bgRenderingTimer);
			iResumeTimer(spawnTimer);
			iResumeTimer(scoreTimer);
		}
	}
}

void HealthBar()
{
	iSetColor(160, 160, 160);
	iFilledRectangle(84, 530, 225, 34);

	if(health > 180)
		iSetColor(0, 204, 0);
	else if(health > 60)
		iSetColor(220, 220, 0);
	else
		iSetColor(204, 0, 0);

	iFilledRectangle(84, 530, health, 34);
	iShowImage(10, 515, 300, 75, healthbarImage);

	if(!nameInput)
	{
		iSetColor(255, 255, 255);
		iText(33, 505, playerName, GLUT_BITMAP_9_BY_15);
	}
}

void BossHealthBar()
{
	iSetColor(160, 160, 160);
	iFilledRectangle(boss.x - 3, boss.y + 180 - 3, 166, 26);
	iSetColor(180, 0, 0);
	iFilledRectangle(boss.x, boss.y + 180, boss.health, 20);
}

void CollisionCheck()
{
	if(fireball[0].show)
	{
		if(setA[1].show)
		{
			if(fireball[0].x >= setA[1].x && fireball[0].x <= (setA[1].x + setA[1].width))
			{
				setA[1].show = false;
				fireball[0].show = false;
				fireball[0].x = 460;
				playerScore += 50;
			}
		}
		if(setB[1].show)
		{
			if(fireball[0].x >= setB[1].x && fireball[0].x <= (setB[1].x + setB[1].width))
			{
				setB[1].show = false;
				fireball[0].show = false;
				fireball[0].x = 460;
				playerScore += 50;
			}
		}
	}
	if(fireball[1].show)
	{
		if(setA[3].show)
		{
			if(fireball[1].x >= setA[3].x && fireball[1].x <= (setA[3].x + setA[3].width))
			{
				setA[3].show = false;
				fireball[1].show = false;
				fireball[1].x = 460;
				playerScore += 50;
			}
		}
		if(setB[3].show)
		{
			if(fireball[1].x >= setB[3].x && fireball[1].x <= (setB[3].x + setB[3].width))
			{
				setB[3].show = false;
				fireball[1].show = false;
				fireball[1].x = 460;
				playerScore += 50;
			}
		}
	}

	if(attacking)
	{
		if(setA[2].show)
		{
			if(setA[2].x >= 460 && setA[2].x <= 475 && attackImageNo >= 6 && attackImageNo <= 9)
			{
				setA[2].show = false;
				playerScore += 100;
			}
		}
		if(setB[2].show)
		{
			if(setB[2].x >= 460 && setB[2].x <= 475 && attackImageNo >= 6 && attackImageNo <= 9)
			{
				setB[2].show = false;
				playerScore += 100;
			}
		}
	}

	if(setA[0].show && setA[0].x >= 365 && setA[0].x <= 440 && character_y + 20 <= (setA[0].y + setA[0].height) && collision[0] == 0)
	{
		health -= 45;
		collision[0] = 1;
		
		if(health != 0)
		{
			hurt = true;
		}
	}
	if(setB[0].show && setB[0].x >= 365 && setB[0].x <= 440 && character_y + 20 <= (setB[0].y + setB[0].height) && collision[4] == 0)
	{
		health -= 45;
		collision[4] = 1;

		if(health != 0)
		{
			hurt = true;
		}
	}
	
	if(setA[1].show && setA[1].x >= 395 && setA[1].x <= 450 && collision[1] == 0)
	{
		health -= 45;
		collision[1] = 1;

		if(health != 0)
		{
			hurt = true;
		}
	}
	if(setB[1].show && setB[1].x >= 395 && setB[1].x <= 450 && collision[5] == 0)
	{
		health -= 45;
		collision[5] = 1;

		if(health != 0)
		{
			hurt = true;
		}
	}

	if(setA[2].show && setA[2].x >= 395 && setA[2].x <= 440 && collision[2] == 0)
	{
		health -= 45;
		collision[2] = 1;

		if(health != 0)
		{
			hurt = true;
		}
	}
	if(setB[2].show && setB[2].x >= 395 && setB[2].x <= 440 && collision[6] == 0)
	{
		health -= 45;
		collision[6] = 1;

		if(health != 0)
		{
			hurt = true;
		}
	}

	if(setA[3].show && setA[3].x >= 395 && setA[3].x <= 420 && collision[3] == 0)
	{
		health -= 45;
		collision[3] = 1;

		if(health != 0)
		{
			hurt = true;
		}
	}
	if(setB[3].show && setB[3].x >= 395 && setB[3].x <= 420 && collision[7] == 0)
	{
		health -= 45;
		collision[7] = 1;

		if(health != 0)
		{
			hurt = true;
		}
	}

	if(bossStage)
	{
		for(int i = 0; i < 2; i++)
		{
			if(boss.show && fireball[i].show)
			{
				if(fireball[i].x >= boss.x && fireball[i].x <= boss.x + boss.width && fireball[i].y >= boss.y && fireball[i].y <= boss.y + boss.health)
				{
					boss.health -= 10;
					fireball[i].show = false;
					fireball[i].x = 460;
				}
			}
		}

		if(bossFire.show)
		{
			if(bossFire.x <= 440)
			{
				health -= 45;
				bossFire.show = false;
				bossFire.x = 780;

				if(health != 0)
				{
					hurt = true;
				}
			}
		}

		if(attacking)
		{
			if(bossFire.show)
			{
				if(bossFire.x >= 460 && bossFire.x <= 475 && attackImageNo >= 6 && attackImageNo <= 9)
				{
					bossFire.show = false;
					bossFire.x = 780;
					playerScore += 100;
				}
			}
		}

		if(life.show)
		{
			if(life.x >= 365 && life.x <= 440 && yFixed)
			{
				if(health != 225)
				{
					health += 45;
				}

				life.x = 5000;
			}
		}
	}
}

void BossInitialize()
{
	bossStage = true;
	iPauseTimer(spawnTimer);

	boss.x = 2000;
	boss.y = 300;
	boss.width = 180;
	boss.height = 160;
	boss.health = 160;
	boss.show = true;
	boss.defeated = false;

	bossFire.x = 780;
	bossFire.y = 140;
	bossFire.show = false;

	life.x = 5000;
	life.y = 250;
	life.width = 50;
	life.height = 48;
	life.show = true;
}

void ShowScore()
{
	iSetColor(30, 30, 30);
	iFilledRectangle(880, 525, 100, 54);
	iShowImage(785, 525, 200, 54, ScoreImage);
	sprintf(scoreStr,"%d", playerScore);
	iSetColor(255, 255, 255);

	if(strlen(scoreStr) == 1)
		iText(923, 543, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
	else if(strlen(scoreStr) == 2)
		iText(915, 543, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
	else if(strlen(scoreStr) == 3)
		iText(910, 543, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
	else if(strlen(scoreStr) == 4)
		iText(905, 543, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
	else if(strlen(scoreStr) == 5)
		iText(900, 543, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
	else
		iText(895, 543, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
}

void ScoreIncrease()
{
	playerScore += 10;
}

void ShowGameOver()
{
	iShowImage(0, 0, 1000, 600, bgImage[5]);
	if(boss.defeated)
		iShowImage(0, 0, 1000, 600, winImage);
	iShowImage(280, 120, 170, 50, buttonImage[6]);
	iShowImage(550, 120, 170, 50, buttonImage[7]);

	if(strlen(scoreStr) == 1)
		iText(490, 260, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
	else if(strlen(scoreStr) == 2)
		iText(482, 260, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
	else if(strlen(scoreStr) == 3)
		iText(477, 260, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
	else if(strlen(scoreStr) == 4)
		iText(472, 260, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
	else if(strlen(scoreStr) == 5)
		iText(467, 260, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
	else
		iText(462, 260, scoreStr, GLUT_BITMAP_TIMES_ROMAN_24);

	if(playAgainHighlight)
	{
		iRectangle(277, 117, 176, 56);
	}
	if(backtoMenuHighlight)
	{
		iRectangle(547, 117, 176, 56);
	}
}

void GameOver()
{
	sprintf(scoreStr,"%d", playerScore);

	character_y = 78;

	bgMovement = false;
	running = false;
	runImageNo = 0;
	jumping = false;
	jumpImageNo = 0;
	jumpY = 0;
	yFixed = false;
	attacking = false;
	attackImageNo = 0;
	jumpFiring = false;
	jumpFiringImageNo = 0;
	if(!boss.defeated)
		dying = true;

	bossStage = false;

	iPauseTimer(bgRenderingTimer);
	iPauseTimer(spawnTimer);
	iPauseTimer(scoreTimer);
}

void QuitGame()
{
	quitGameBox = false;
	playWindow = false;
	mainWindow = true;
	paused = false;

	character_y = 78;
	playerScore = 0;
	health = 225;

	bgMovement = false;
	running = false;
	runImageNo = 0;
	jumping = false;
	jumpImageNo = 0;
	jumpY = 0;
	yFixed = false;
	attacking = false;
	attackImageNo = 0;
	jumpFiring = false;
	jumpFiringImageNo = 0;
	dying = false;
	dieImageNo = 0;
	hurt = false;
	characterShow = 0;

	bossStage = false;

	iResumeTimer(characterAnimationTimer);
	iResumeTimer(jumpAnimationTimer);
	iResumeTimer(attackAnimationTimer);
}

void BgRendering()
{
	if(bgMovement)
	{
		if(bg1_x >= -990)
		{
			bg1_x -= SPEED;
		}
		else
		{
			bg1_x = 990;
		}

		if(bg2_x >= -990)
		{
			bg2_x -= SPEED;
		}
		else
		{
			bg2_x = 990;
		}

		if(setA[0].x >= (0 - setA[0].width) && setA[0].show)
		{
			setA[0].x -= SPEED;
		}
		else
		{
			setA[0].show = false;
		}
		if(setB[0].x >= (0 - setB[0].width) && setB[0].show)
		{
			setB[0].x -= SPEED;
		}
		else
		{
			setB[0].show = false;
		}

		if(setA[1].x >= (0 - setA[1].width) && setA[1].show)
		{
			setA[1].x -= SPEED;
		}
		else
		{
			setA[1].show = false;
		}
		if(setB[1].x >= (0 - setB[1].width) && setB[1].show)
		{
			setB[1].x -= SPEED;
		}
		else
		{
			setB[1].show = false;
		}
	}

	if(setA[2].x >= (0 - setA[2].width) && setA[2].x <= 1000 && setA[2].show)
	{
		setA[2].x -= SPEED + 1;
	}
	else if(setA[2].x > 1000 && setA[2].show)
	{
		setA[2].x -= SPEED;
	}
	else
	{
		setA[2].show = false;
	}

	if(setB[2].x >= (0 - setB[2].width) && setB[2].x <= 1000 && setB[2].show)
	{
		setB[2].x -= SPEED + 1;
	}
	else if(setB[2].x > 1000 && setB[2].show)
	{
		setB[2].x -= SPEED;
	}
	else
	{
		setB[2].show = false;
	}

	if(setA[3].x >= (0 - setA[3].width) && setA[3].x <= 1000 && setA[3].show)
	{
		setA[3].x -= SPEED + 1;
	}
	else if(setA[3].x > 1000 && setA[3].show)
	{
		setA[3].x -= SPEED;
	}
	else
	{
		setA[3].show = false;
	}

	if(setB[3].x >= (0 - setB[3].width) && setB[3].x <= 1000 && setB[3].show)
	{
		setB[3].x -= SPEED + 1;
	}
	else if(setB[3].x > 1000 && setB[3].show)
	{
		setB[3].x -= SPEED;
	}
	else
	{
		setB[3].show = false;
	}

	if(fireball[0].x <= 1055 && fireball[0].show)
	{
		fireball[0].x += SPEED + 1;
	}
	else
	{
		fireball[0].show = false;
		fireball[0].x = 460;
	}
	if(fireball[1].x <= 1055 && fireball[1].show)
	{
		fireball[1].x += SPEED + 1;
	}
	else
	{
		fireball[1].show = false;
		fireball[1].x = 460;
	}

	if(bossStage)
	{
		if(boss.show)
		{
			if(boss.x >= 820)
			{
				boss.x -= SPEED - 1;
			}
			if(boss.x <= 820)
			{
				boss.y += dy;
			}
			if(boss.y > 300 || boss.y < 90)
			{
				dy = -dy;
			}
			if(boss.y == 90)
			{
				bossFire.show = true;
			}
		}

		if(bossFire.show)
		{
			if(bossFire.x >= 420)
			{
				bossFire.x -= SPEED + 3;
			}
			else
			{
				bossFire.x = 780;
				bossFire.show = false;
			}
		}

		if(life.show)
		{
			if(life.x >= 0 - life.width)
			{
				life.x -= SPEED + 1;
			}
			else
			{
				life.x = 5000;
			}
		}
	}
}

void MainWindow()
{
	iShowImage(0, 0, 1000, 600, bgImage[0]);

	iShowImage(415, 350, 170, 50, buttonImage[0]);
	iShowImage(415, 280, 170, 50, buttonImage[1]);
	iShowImage(415, 210, 170, 50, buttonImage[2]);
	iShowImage(415, 140, 170, 50, buttonImage[3]);
	iShowImage(415, 70, 170, 50, buttonImage[4]);

	if(musicOn)
	{
		if(!menuUnmutedPop)
		{
			iShowImage(900, 70, 50, 50, iconImage[0]);
		}
		else
		{
			iShowImage(898, 68, 54, 54, iconImage[1]);
		}
	}
	else
	{
		if(!menuMutedPop)
		{
			iShowImage(900, 70, 50, 50, iconImage[2]);
		}
		else
		{
			iShowImage(898, 68, 54, 54, iconImage[3]);
		}
	}

	if(playHighlight)
	{
		iRectangle(412, 347, 176, 56);
	}
	if(hSHighlight)
	{
		iRectangle(412, 277, 176, 56);
	}
	if(hTPHighlight)
	{
		iRectangle(412, 207, 176, 56);
	}
	if(aUHighlight)
	{
		iRectangle(412, 137, 176, 56);
	}
	if(exitHighlight)
	{
		iRectangle(412, 67, 176, 56);
	}
}

void PlayWindow()
{
	iShowImage(bg1_x, 0, 1000, 600, bgImage[1]);
	iShowImage(bg2_x, 0, 1000, 600, bgImage[1]);

	if(nameInput)
	{
		NameInputBox();
	}
	else
	{
		HealthBar();
		ShowScore();
	}

	if(counting)
	{
		iShowImage(400, 250, 140, 180, countImage[countImageNo]);
	}
	
	if(!nameInput && !counting)
	{
		if(musicOn)
		{
			if(!unmutedPop)
			{
				iShowImage(420, 525, 50, 50, iconImage[4]);
			}
			else
			{
				iShowImage(418, 523, 54, 54, iconImage[5]);
			}
		}
		else
		{
			if(!mutedPop)
			{
				iShowImage(420, 525, 50, 50, iconImage[6]);
			}
			else
			{
				iShowImage(418, 523, 54, 54, iconImage[7]);
			}
		}

		if(!paused)
		{
			if(!pausePop)
			{
				iShowImage(520, 525, 50, 50, iconImage[8]);
			}
			else
			{
				iShowImage(518, 523, 54, 54, iconImage[9]);
			}
		}
		else
		{
			if(!resumePop)
			{
				iShowImage(520, 525, 50, 50, iconImage[10]);
			}
			else
			{
				iShowImage(518, 523, 54, 54, iconImage[11]);
			}
		}

		if(!quitPop)
		{
			iShowImage(620, 525, 50, 50, iconImage[12]);
		}
		else
		{
			iShowImage(618, 523, 54, 54, iconImage[13]);
		}
	}

	if(idling)
	{
		iShowImage(character_x, character_y, 250, 150, idleImage[idleImageNo]);
	}

	else if(running && (characterShow % 2 == 0))
	{
		iShowImage(character_x, character_y, 250, 150, runImage[runImageNo]);
	}

	else if(jumping && (characterShow % 2 == 0))
	{
		iShowImage(character_x, character_y, 250, 150, jumpImage[jumpImageNo]);
		
	}

	else if(attacking && (characterShow % 2 == 0))
	{
		iShowImage(character_x, character_y, 250, 150, attackImage[attackImageNo]);
	}

	else if(jumpFiring && (characterShow % 2 == 0))
	{
		iShowImage(character_x, character_y, 250, 150, jumpImage[jumpFiringImageNo]);
	}

	else if(dying)
	{
		iShowImage(character_x, character_y, 250, 150, dieImage[dieImageNo]);
	}

	if(setA[0].show)
	{
		iShowImage(setA[0].x, setA[0].y, setA[0].width, setA[0].height, obsImage[0]);
	}

	if(setB[0].show)
	{
		iShowImage(setB[0].x, setB[0].y, setB[0].width, setB[0].height, obsImage[0]);
	}

	if(setA[1].show)
	{
		iShowImage(setA[1].x, setA[1].y, setA[1].width, setA[1].height, obsImage[1]);
	}

	if(setB[1].show)
	{
		iShowImage(setB[1].x, setB[1].y, setB[1].width, setB[1].height, obsImage[1]);
	}

	if(setA[2].show)
	{
		iShowImage(setA[2].x, setA[2].y, setA[2].width, setA[2].height, zombieImage[zombieAImageNo]);
	}

	if(setB[2].show)
	{
		iShowImage(setB[2].x, setB[2].y, setB[2].width, setB[2].height, zombieImage[zombieBImageNo]);
	}

	if(setA[3].show)
	{
		iShowImage(setA[3].x, setA[3].y, setA[3].width, setA[3].height, batImage[batAImageNo]);
	}

	if(setB[3].show)
	{
		iShowImage(setB[3].x, setB[3].y, setB[3].width, setB[3].height, batImage[batBImageNo]);
	}

	if(fireball[0].show)
	{
		iShowImage(fireball[0].x, fireball[0].y, 55, 30, fireBallImage);
	}
	if(fireball[1].show)
	{
		iShowImage(fireball[1].x, fireball[1].y, 55, 30, fireBallImage);
	}

	if(quitGameBox)
	{
		QuitGameBox();
	}

	if(bossStage)
	{
		if(boss.show)
		{
			iShowImage(boss.x, boss.y, boss.width, boss.height, bossImage);
			BossHealthBar();
		}
		if(bossFire.show)
		{
			iShowImage(bossFire.x, bossFire.y, 78, 50, bossFireImage);
		}
		if(life.show)
		{
			iShowImage(life.x, life.y, life.width, life.height, lifeImage);
		}
	}
}

void HSWindow()
{
	iShowImage(0, 0, 1000, 600, bgImage[2]);

	iShowImage(870, 540, 115, 50, buttonImage[5]);

	if(backHighlight)
	{
		iRectangle(867, 537, 121, 56);
	}
}

void HTPWindow()
{
	iShowImage(0, 0, 1000, 600, bgImage[3]);

	iShowImage(870, 540, 115, 50, buttonImage[5]);

	if(backHighlight)
	{
		iRectangle(867, 537, 121, 56);
	}
}

void AboutUsWindow()
{
	iShowImage(0, 0, 1000, 600, bgImage[4]);

	iShowImage(870, 540, 115, 50, buttonImage[5]);

	if(backHighlight)
	{
		iRectangle(867, 537, 121, 56);
	}
}

void HurtAnimation()
{
	if(hurt)
	{
		if(characterShow < 10)
		{
			characterShow++;
		}
		else
		{
			hurt = false;
			characterShow = 0;
		}
	}
}

void CharacterAnimation()
{
	if(idling)
	{
		if(idleImageNo < 9)
		{
			idleImageNo++;
		}
		else
		{
			idleImageNo = 0;
		}
	}

	if(running)
	{
		if(runImageNo < 9)
		{
			runImageNo++;
		}
		else
		{
			runImageNo = 0;
		}

		HurtAnimation();
	}

	if(dying)
	{
		if(dieImageNo < 9)
		{
			dieImageNo++;
		}
		else
		{
			dieImageNo = 0;
			dying = false;

			playWindow = false;
			gameOver = true;

			GetHighScores();
			SaveHighScore();
			totalHighScores = 0;

			playerScore = 0;
			health = 225;

			if(musicOn)
			{
				PlaySound("Sound//menumusic.wav", NULL, SND_ASYNC | SND_LOOP);
			}
		}
	}

	if(jumpFiring)
	{
		if(jumpFiringImageNo < 9)
		{
			jumpFiringImageNo++;
		}
		else
		{
			jumpFiringImageNo = 0;
			jumpFiring = false;
			running = true;
		}

		if(jumpFiringImageNo >= 1 && jumpFiringImageNo <= 4)
		{
			character_y += 15;
		}
		else if(jumpFiringImageNo >= 5 && jumpFiringImageNo <= 8)
		{
			character_y -= 15;
		}

		if(jumpFiringImageNo == 4)
		{
			fireball[1].show = true;
		}

		HurtAnimation();
	}

	if(setA[2].show)
	{
		if(zombieAImageNo < 4)
		{
			zombieAImageNo++;
		}
		else
		{
			zombieAImageNo = 0;
		}
	}
	if(setB[2].show)
	{
		if(zombieBImageNo < 4)
		{
			zombieBImageNo++;
		}
		else
		{
			zombieBImageNo = 0;
		}
	}

	if(setA[3].show)
	{
		if(batAImageNo < 3)
		{
			batAImageNo++;
		}
		else
		{
			batAImageNo = 0;
		}
	}
	if(setB[3].show)
	{
		if(batBImageNo < 3)
		{
			batBImageNo++;
		}
		else
		{
			batBImageNo = 0;
		}
	}
}

void JumpAnimation()
{
	if(jumping)
	{
		if(jumpImageNo < 9)
		{
			jumpY++;

			if(jumpY == 5)
			{
				yFixed = true;
			}
			if(jumpY == 9)
			{
				yFixed = false;
			}
			if(!yFixed)
			{
				jumpImageNo++;
			}
		}
		else
		{
			jumpImageNo = 0;
			jumpY = 0;
			yFixed = false;
			jumping = false;
			running = true;
		}

		if(jumpImageNo >= 1 && jumpImageNo <= 4 && !(yFixed))
		{
			character_y += 18;
		}
		else if(jumpImageNo >= 5 && jumpImageNo <= 8 && !(yFixed))
		{
			character_y -= 18;
		}

		HurtAnimation();
	}
}

void AttackAnimation()
{
	if(attacking)
	{
		if(attackImageNo < 9)
		{
			attackImageNo++;
		}
		else
		{
			attackImageNo = 0;
			attacking = false;
			running = true;

			bgMovement = true;
		}

		HurtAnimation();
	}
}

int matchNumber(int num, int array[])
{
    int i;

    for(i = 0; i < 3; i++)
    {
        if(num == array[i])
        {
            return 1;
        }
    }

    return 0;
}

void RandomSequence()
{
	int num, i = 0, j, upper = 3, lower = 0;

	for(j = 0; j < 4; j++)
	{
		randomSeq[j] = -1;
	}

	srand(time(0));

    while (1)
    {
        num = (rand() % (upper - lower + 1)) + lower;

        if(!matchNumber(num, randomSeq))
        {
            randomSeq[i] = num;
            i++;
        }

        if(i == 4)
            break;
    }
}

void EnemyObstacleInitialize()
{
	set = 'A';

	setA[0].x = 0;
	setA[0].y = 88;
	setA[0].height = 55;
	setA[0].width = 59;
	setA[0].show = false;

	setA[1].x = 0;
	setA[1].y = 88;
	setA[1].height = 100;
	setA[1].width = 35;
	setA[1].show = false;

	setA[2].x = 0;
	setA[2].y = 88;
	setA[2].height = 120;
	setA[2].width = 110;
	setA[2].show = false;

	setA[3].x = 0;
	setA[3].y = 176;
	setA[3].height = 78;
	setA[3].width = 90;
	setA[3].show = false;

	setB[0].x = 0;
	setB[0].y = 88;
	setB[0].height = 55;
	setB[0].width = 59;
	setB[0].show = false;

	setB[1].x = 0;
	setB[1].y = 88;
	setB[1].height = 100;
	setB[1].width = 35;
	setB[1].show = false;

	setB[2].x = 0;
	setB[2].y = 88;
	setB[2].height = 120;
	setB[2].width = 110;
	setB[2].show = false;

	setB[3].x = 0;
	setB[3].y = 176;
	setB[3].height = 78;
	setB[3].width = 90;
	setB[3].show = false;

	fireball[0].x = 460;
	fireball[0].y = 140;
	fireball[0].show = false;

	fireball[1].x = 460;
	fireball[1].y = 210;
	fireball[1].show = false;
}

void EnemyObstacleSpawn()
{
	RandomSequence();

	if(set == 'A')
	{
		setA[randomSeq[0]].x = 1000;
		setA[randomSeq[1]].x = 1400;
		setA[randomSeq[2]].x = 1800;
		setA[randomSeq[3]].x = 2200;

		setA[0].show = true;
		setA[1].show = true;
		setA[2].show = true;
		setA[3].show = true;

		collision[0] = 0;
		collision[1] = 0;
		collision[2] = 0;
		collision[3] = 0;

		set = 'B';
	}
	else
	{
		setB[randomSeq[0]].x = 1000;
		setB[randomSeq[1]].x = 1400;
		setB[randomSeq[2]].x = 1800;
		setB[randomSeq[3]].x = 2200;

		setB[0].show = true;
		setB[1].show = true;
		setB[2].show = true;
		setB[3].show = true;

		collision[4] = 0;
		collision[5] = 0;
		collision[6] = 0;
		collision[7] = 0;

		set = 'A';
	}
}

void iDraw()
{
	iClear();

	iSetColor(255, 255, 255);

	if(mainWindow)
	{
		MainWindow();
	}

	if(playWindow)
	{
		PlayWindow();

		if(!bossStage && playerScore > scoreLimit)
		{
			BossInitialize();
		}

		if(bossStage)
		{
			if(boss.health == 0)
			{
				boss.defeated = true;
				GameOver();
				playWindow = false;
				gameOver = true;

				GetHighScores();
				SaveHighScore();
				totalHighScores = 0;

				playerScore = 0;
				health = 225;

				if(musicOn)
				{
					PlaySound("Sound//menumusic.wav", NULL, SND_ASYNC | SND_LOOP);
				}
			}
		}
	}

	if(hSWindow)
	{
		HSWindow();
		ShowHighScores();
	}

	if(hTPWindow)
	{
		HTPWindow();
	}

	if(aboutUsWindow)
	{
		AboutUsWindow();
	}

	if(gameOver)
	{
		ShowGameOver();
	}

	CollisionCheck();

	if(health == 0)
	{
		GameOver();
	}
}

void iMouseMove(int mx, int my)
{
	
}

void iPassiveMouse(int mx, int my)
{
	if(mainWindow)
	{
		if(mx >= 415 && mx <= 585 && my >= 350 && my <= 400)
		{
			playHighlight = true;
		}
		else
		{
			playHighlight = false;
		}
		if(mx >= 415 && mx <= 585 && my >= 280 && my <= 330)
		{
			hSHighlight = true;
		}
		else
		{
			hSHighlight = false;
		}
		if(mx >= 415 && mx <= 585 && my >= 210 && my <= 260)
		{
			hTPHighlight = true;
		}
		else
		{
			hTPHighlight = false;
		}
		if(mx >= 415 && mx <= 585 && my >= 140 && my <= 190)
		{
			aUHighlight = true;
		}
		else
		{
			aUHighlight = false;
		}
		if(mx >= 415 && mx <= 585 && my >= 70 && my <= 120)
		{
			exitHighlight = true;
		}
		else
		{
			exitHighlight = false;
		}
		if(mx >= 900 && mx <= 950 && my >= 70 && my <= 120)
		{
			menuUnmutedPop = true;
			menuMutedPop = true;
		}
		else
		{
			menuUnmutedPop = false;
			menuMutedPop = false;
		}
	}

	if(playWindow)
	{
		if(quitGameBox)
		{
			if(mx >= 370 && mx <= 470 && my >= 250 && my <= 290)
			{
				yesHighlight = true;
			}
			else
			{
				yesHighlight = false;
			}
			if(mx >= 530 && mx <= 630 && my >= 250 && my <= 290)
			{
				noHighlight = true;
			}
			else
			{
				noHighlight = false;
			}
		}

		if(mx >= 420 && mx <= 470 && my >= 525 && my <= 575)
		{
			unmutedPop = true;
			mutedPop = true;
		}
		else
		{
			unmutedPop = false;
			mutedPop = false;
		}

		if(mx >= 520 && mx <= 570 && my >= 525 && my <= 575)
		{
			pausePop = true;
			resumePop = true;
		}
		else
		{
			pausePop = false;
			resumePop = false;
		}

		if(mx >= 620 && mx <= 670 && my >= 525 && my <= 575)
		{
			quitPop = true;
		}
		else
		{
			quitPop = false;
		}
	}

	if(hSWindow)
	{
		if(mx >= 870 && mx <= 985 && my >= 540 && my <= 590)
		{
			backHighlight = true;
		}
		else
		{
			backHighlight = false;
		}
	}

	if(hTPWindow)
	{
		if(mx >= 870 && mx <= 985 && my >= 540 && my <= 590)
		{
			backHighlight = true;
		}
		else
		{
			backHighlight = false;
		}
	}
	
	if(aboutUsWindow)
	{
		if(mx >= 870 && mx <= 985 && my >= 540 && my <= 590)
		{
			backHighlight = true;
		}
		else
		{
			backHighlight = false;
		}
	}

	if(gameOver)
	{
		if(mx >= 280 && mx <= 450 && my >= 120 && my <= 170)
		{
			playAgainHighlight = true;
		}
		else
		{
			playAgainHighlight = false;
		}
		if(mx >= 550 && mx <= 720 && my >= 120 && my <= 170)
		{
			backtoMenuHighlight = true;
		}
		else
		{
			backtoMenuHighlight = false;
		}
	}

}

void iMouse(int button, int state, int mx, int my)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if(mainWindow)
		{

			if(mx >= 415 && mx <= 585 && my >= 350 && my <= 400)
			{
				playWindow = true;
				mainWindow = false;
				playHighlight = false;

				nameInput = true;
				idling = true;

				EnemyObstacleInitialize();

				if(musicOn)
				{
					PlaySound("Sound//playmusic.wav", NULL, SND_ASYNC | SND_LOOP);
				}
			}
			if(mx >= 415 && mx <= 585 && my >= 280 && my <= 330)
			{
				hSWindow = true;
				mainWindow = false;
				hSHighlight = false;

				GetHighScores();
			}
			if(mx >= 415 && mx <= 585 && my >= 210 && my <= 260)
			{
				hTPWindow = true;
				mainWindow = false;
				hTPHighlight = false;
			}
			if(mx >= 415 && mx <= 585 && my >= 140 && my <= 190)
			{
				aboutUsWindow = true;
				mainWindow = false;
				aUHighlight = false;
			}
			if(mx >= 415 && mx <= 585 && my >= 70 && my <= 120)
			{
				exit(0);
			}
			if(mx >= 900 && mx <= 950 && my >= 70 && my <= 120)
			{
				if(musicOn)
				{
					PlaySound(0, 0, 0);
					musicOn = false;
				}
				else
				{
					PlaySound("Sound//menumusic.wav", NULL, SND_ASYNC | SND_LOOP);
					musicOn = true;
				}
			}
		}

		if(playWindow)
		{
			if(nameInput)
			{
				if(mx >= 400 && mx <= 600 && my >= 300 && my <=350)
				{
					inputMode = true;
				}
			}

			if(quitGameBox)
			{
				if(mx >= 370 && mx <= 470 && my >= 250 && my <= 290)
				{
					QuitGame();
					yesHighlight = false;

					if(musicOn)
					{
						PlaySound("Sound//menumusic.wav", NULL, SND_ASYNC | SND_LOOP);
					}
				}
				
				if(mx >= 530 && mx <= 630 && my >= 250 && my <= 290)
				{
					quitGameBox = false;
					iResumeTimer(bgRenderingTimer);
					iResumeTimer(characterAnimationTimer);
					iResumeTimer(jumpAnimationTimer);
					iResumeTimer(attackAnimationTimer);
					if(!bossStage)
						iResumeTimer(spawnTimer);
					iResumeTimer(scoreTimer);
					paused = false;
					noHighlight = false;
				}
			}

			if(!nameInput && !counting)
			{
				if(mx >= 420 && mx <= 470 && my >= 525 && my <= 575)
				{
					if(musicOn)
					{
						PlaySound(0, 0, 0);
						musicOn = false;
					}
					else
					{
						PlaySound("Sound//playmusic.wav", NULL, SND_ASYNC | SND_LOOP);
						musicOn = true;
					}
				}
				
				if(mx >= 520 && mx <= 570 && my >= 525 && my <= 575)
				{
					if(!quitGameBox)
					{
						if(paused)
						{
							iResumeTimer(bgRenderingTimer);
							iResumeTimer(characterAnimationTimer);
							iResumeTimer(jumpAnimationTimer);
							iResumeTimer(attackAnimationTimer);
							iResumeTimer(spawnTimer);
							iResumeTimer(scoreTimer);
							paused = false;
						}
						else
						{
							iPauseTimer(bgRenderingTimer);
							iPauseTimer(characterAnimationTimer);
							iPauseTimer(jumpAnimationTimer);
							iPauseTimer(attackAnimationTimer);
							iPauseTimer(spawnTimer);
							iPauseTimer(scoreTimer);
							paused = true;
						}
					}
				}
				
				if(mx >= 620 && mx <= 670 && my >= 525 && my <= 575)
				{
					if(!paused)
					{
						iPauseTimer(bgRenderingTimer);
						iPauseTimer(characterAnimationTimer);
						iPauseTimer(jumpAnimationTimer);
						iPauseTimer(attackAnimationTimer);
						iPauseTimer(spawnTimer);
						iPauseTimer(scoreTimer);
						paused = true;
					}

					quitGameBox = true;
				}
			}
		}

		if(hSWindow)
		{

			if(mx >= 870 && mx <= 985 && my >= 540 && my <= 590)
			{
				mainWindow = true;
				hSWindow = false;
				backHighlight = false;

				totalHighScores = 0;
			}

		}

		if(hTPWindow)
		{

			if(mx >= 870 && mx <= 985 && my >= 540 && my <= 590)
			{
				mainWindow = true;
				hTPWindow = false;
				backHighlight = false;
			}

		}

		if(aboutUsWindow)
		{

			if(mx >= 870 && mx <= 985 && my >= 540 && my <= 590)
			{
				mainWindow = true;
				aboutUsWindow = false;
				backHighlight = false;
			}

		}

		if(gameOver)
		{

			if(mx >= 280 && mx <= 450 && my >= 120 && my <= 170)
			{
				playWindow = true;
				gameOver = false;
				playAgainHighlight = false;

				idling = true;
				counting = true;
				iResumeTimer(countTimer);

				EnemyObstacleInitialize();

				if(boss.defeated)
					boss.defeated = false;

				if(musicOn)
				{
					PlaySound("Sound//playmusic.wav", NULL, SND_ASYNC | SND_LOOP);
				}
			}
		
			if(mx >= 550 && mx <= 720 && my >= 120 && my <= 170)
			{
				if(boss.defeated)
					boss.defeated = false;

				mainWindow = true;
				gameOver = false;
				backtoMenuHighlight = false;
			}

		}
	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		
	}
}

void iKeyboard(unsigned char key)
{
	int i;

	if(playWindow)
	{
		if(inputMode)
		{
			if(key == '\r')
			{
				if(nameLength != 0)
				{
					strcpy(playerName, nameStr);
				
					for(i = 0; i < nameLength; i++)
					{
						nameStr[i] = '\0';
					}
					nameLength = 0;
				}
				else
				{
					strcpy(playerName, "Unknown");
				}

				inputMode = false;
				nameInput = false;
				counting = true;
				iResumeTimer(countTimer);
			}
			else if(key == '\b')
			{
				if(nameLength != 0)
				{
					nameStr[nameLength - 1] = '\0';
					nameLength--;
				}
			}
			else
			{
				if(nameLength <= 9)
				{
					nameStr[nameLength] = key;
					nameLength++;
				}
			}
		}
		else
		{
			if(key == 'p')
			{
				if(!counting && !quitGameBox)
				{
					if(paused)
					{
						iResumeTimer(bgRenderingTimer);
						iResumeTimer(characterAnimationTimer);
						iResumeTimer(jumpAnimationTimer);
						iResumeTimer(attackAnimationTimer);
						if(!bossStage)
							iResumeTimer(spawnTimer);
						iResumeTimer(scoreTimer);
						paused = false;
					}
					else
					{
						iPauseTimer(bgRenderingTimer);
						iPauseTimer(characterAnimationTimer);
						iPauseTimer(jumpAnimationTimer);
						iPauseTimer(attackAnimationTimer);
						iPauseTimer(spawnTimer);
						iPauseTimer(scoreTimer);
						paused = true;
					}
				}
			}

			if(!paused && !counting)
			{
				if(key == 'w')
				{
					if(!attacking && !jumpFiring)
					{
						jumping = true;
						running = false;
					}
				}

				if(key == 'a')
				{
					if(!jumping && !jumpFiring)
					{
						attacking = true;
						running = false;
					}
				}

				if(key == 'd')
				{
					if(!attacking && !jumping && !jumpFiring && character_y == 78)
					{
						fireball[0].show = true;
					}
				}

				if(key == 'e')
				{
					if(!attacking && !jumping && fireball[1].x == 460)
					{
						jumpFiring = true;
						running = false;
					}
				}
			}
			
			if(!counting)
			{
				if(key == 'x')
				{
					if(!paused)
					{
						iPauseTimer(bgRenderingTimer);
						iPauseTimer(characterAnimationTimer);
						iPauseTimer(jumpAnimationTimer);
						iPauseTimer(attackAnimationTimer);
						iPauseTimer(spawnTimer);
						iPauseTimer(scoreTimer);
						paused = true;
					}

					quitGameBox = true;
				}
			}
		}
	}

	if(!nameInput)
	{
		if(key == 'm')
		{
			if(musicOn)
			{
				PlaySound(0, 0, 0);
				musicOn = false;
			}
			else
			{
				if(playWindow)
				{
					PlaySound("Sound//playmusic.wav", NULL, SND_ASYNC | SND_LOOP);
				}
				else
				{
					PlaySound("Sound//menumusic.wav", NULL, SND_ASYNC | SND_LOOP);
				}
				musicOn = true;
			}
		}
	}
}

void iSpecialKeyboard(unsigned char key)
{
	if(playWindow)
	{
		if(key == GLUT_KEY_END)
		{
			exit(0);
		}
	}
}

int main()
{
	iInitialize(SCRN_WIDTH, SCRN_HEIGHT, "Devil Slayer : The Justice Resurrection");

	bgImage[0] = iLoadImage("Backgrounds//MainMenuBg.bmp");
	bgImage[1] = iLoadImage("Backgrounds//GameBg.bmp");
	bgImage[2] = iLoadImage("Backgrounds//HighScoresBg.bmp");
	bgImage[3] = iLoadImage("Backgrounds//HowToPlayBg.bmp");
	bgImage[4] = iLoadImage("Backgrounds//AboutUsBg.bmp");
	bgImage[5] = iLoadImage("Backgrounds//GameOverBg.bmp");

	frameImage = iLoadImage("Miscellaneous//frame.png");
	healthbarImage = iLoadImage("Miscellaneous//healthbar.png");
	ScoreImage = iLoadImage("Miscellaneous//score.png");
	noHighScores = iLoadImage("Miscellaneous//nohighscores.png");
	quitImage = iLoadImage("Miscellaneous//quit.png");

	countImage[0] = iLoadImage("Miscellaneous//number1.png");
	countImage[1] = iLoadImage("Miscellaneous//number2.png");
	countImage[2] = iLoadImage("Miscellaneous//number3.png");

	buttonImage[0] = iLoadImage("Buttons//Play.bmp");
	buttonImage[1] = iLoadImage("Buttons//HighScores.bmp");
	buttonImage[2] = iLoadImage("Buttons//HowtoPlay.bmp");
	buttonImage[3] = iLoadImage("Buttons//AboutUs.bmp");
	buttonImage[4] = iLoadImage("Buttons//Exit.bmp");
	buttonImage[5] = iLoadImage("Buttons//Back.bmp");
	buttonImage[6] = iLoadImage("Buttons//PlayAgain.png");
	buttonImage[7] = iLoadImage("Buttons//BacktoMenu.png");
	buttonImage[8] = iLoadImage("Buttons//Yes.png");
	buttonImage[9] = iLoadImage("Buttons//No.png");

	iconImage[0] = iLoadImage("Buttons//MenuUnmuted.png");
	iconImage[1] = iLoadImage("Buttons//MenuUnmuted2.png");
	iconImage[2] = iLoadImage("Buttons//MenuMuted.png");
	iconImage[3] = iLoadImage("Buttons//MenuMuted2.png");
	iconImage[4] = iLoadImage("Buttons//Unmuted.png");
	iconImage[5] = iLoadImage("Buttons//Unmuted2.png");
	iconImage[6] = iLoadImage("Buttons//Muted.png");
	iconImage[7] = iLoadImage("Buttons//Muted2.png");
	iconImage[8] = iLoadImage("Buttons//Pause.png");
	iconImage[9] = iLoadImage("Buttons//Pause2.png");
	iconImage[10] = iLoadImage("Buttons//Resume.png");
	iconImage[11] = iLoadImage("Buttons//Resume2.png");
	iconImage[12] = iLoadImage("Buttons//Quit.png");
	iconImage[13] = iLoadImage("Buttons//Quit2.png");

	idleImage[0] = iLoadImage("Character//idle0.png");
	idleImage[1] = iLoadImage("Character//idle1.png");
	idleImage[2] = iLoadImage("Character//idle2.png");
	idleImage[3] = iLoadImage("Character//idle3.png");
	idleImage[4] = iLoadImage("Character//idle4.png");
	idleImage[5] = iLoadImage("Character//idle5.png");
	idleImage[6] = iLoadImage("Character//idle6.png");
	idleImage[7] = iLoadImage("Character//idle7.png");
	idleImage[8] = iLoadImage("Character//idle8.png");
	idleImage[9] = iLoadImage("Character//idle9.png");

	runImage[0] = iLoadImage("Character//run0.png");
	runImage[1] = iLoadImage("Character//run1.png");
	runImage[2] = iLoadImage("Character//run2.png");
	runImage[3] = iLoadImage("Character//run3.png");
	runImage[4] = iLoadImage("Character//run4.png");
	runImage[5] = iLoadImage("Character//run5.png");
	runImage[6] = iLoadImage("Character//run6.png");
	runImage[7] = iLoadImage("Character//run7.png");
	runImage[8] = iLoadImage("Character//run8.png");
	runImage[9] = iLoadImage("Character//run9.png");

	jumpImage[0] = iLoadImage("Character//jump0.png");
	jumpImage[1] = iLoadImage("Character//jump1.png");
	jumpImage[2] = iLoadImage("Character//jump2.png");
	jumpImage[3] = iLoadImage("Character//jump3.png");
	jumpImage[4] = iLoadImage("Character//jump4.png");
	jumpImage[5] = iLoadImage("Character//jump5.png");
	jumpImage[6] = iLoadImage("Character//jump6.png");
	jumpImage[7] = iLoadImage("Character//jump7.png");
	jumpImage[8] = iLoadImage("Character//jump8.png");
	jumpImage[9] = iLoadImage("Character//jump9.png");

	attackImage[0] = iLoadImage("Character//attack0.png");
	attackImage[1] = iLoadImage("Character//attack1.png");
	attackImage[2] = iLoadImage("Character//attack2.png");
	attackImage[3] = iLoadImage("Character//attack3.png");
	attackImage[4] = iLoadImage("Character//attack4.png");
	attackImage[5] = iLoadImage("Character//attack5.png");
	attackImage[6] = iLoadImage("Character//attack6.png");
	attackImage[7] = iLoadImage("Character//attack7.png");
	attackImage[8] = iLoadImage("Character//attack8.png");
	attackImage[9] = iLoadImage("Character//attack9.png");

	dieImage[0] = iLoadImage("Character//die0.png");
	dieImage[1] = iLoadImage("Character//die1.png");
	dieImage[2] = iLoadImage("Character//die2.png");
	dieImage[3] = iLoadImage("Character//die3.png");
	dieImage[4] = iLoadImage("Character//die4.png");
	dieImage[5] = iLoadImage("Character//die5.png");
	dieImage[6] = iLoadImage("Character//die6.png");
	dieImage[7] = iLoadImage("Character//die7.png");
	dieImage[8] = iLoadImage("Character//die8.png");
	dieImage[9] = iLoadImage("Character//die9.png");

	obsImage[0] = iLoadImage("Obstacles//obstacle1.png");
	obsImage[1] = iLoadImage("Obstacles//obstacle2.png");

	batImage[0] = iLoadImage("Enemies//bat0.png");
	batImage[1] = iLoadImage("Enemies//bat1.png");
	batImage[2] = iLoadImage("Enemies//bat2.png");
	batImage[3] = iLoadImage("Enemies//bat3.png");

	zombieImage[0] = iLoadImage("Enemies//zombie10.png");
	zombieImage[1] = iLoadImage("Enemies//zombie11.png");
	zombieImage[2] = iLoadImage("Enemies//zombie12.png");
	zombieImage[3] = iLoadImage("Enemies//zombie13.png");
	zombieImage[4] = iLoadImage("Enemies//zombie14.png");

	bossImage = iLoadImage("Enemies//boss.png");
	bossFireImage = iLoadImage("Miscellaneous//bossfire.png");
	winImage = iLoadImage("Backgrounds//winBg.bmp");

	fireBallImage = iLoadImage("Miscellaneous//fireball.png");
	lifeImage = iLoadImage("Miscellaneous//life.png");

	bgRenderingTimer = iSetTimer(1, BgRendering);
	countTimer = iSetTimer(1000, StartCounting);
	characterAnimationTimer = iSetTimer(50, CharacterAnimation);
	jumpAnimationTimer = iSetTimer(75, JumpAnimation);
	attackAnimationTimer = iSetTimer(36, AttackAnimation);
	spawnTimer = iSetTimer(7800, EnemyObstacleSpawn);
	scoreTimer = iSetTimer(2000, ScoreIncrease);
	
	iPauseTimer(bgRenderingTimer);
	iPauseTimer(countTimer);
	iPauseTimer(spawnTimer);
	iPauseTimer(scoreTimer);
	
	if(musicOn)
	{
		PlaySound("Sound//menumusic.wav", NULL, SND_ASYNC | SND_LOOP);
	}

    iStart();

	return 0;
}