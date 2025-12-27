#include "raylib.h"
#include <string>
#include <cmath> 


//Structure of animation
struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runntingTime;
};


//Boolean function to refer to the ground
bool isOnGround(AnimData data, int windowHeight)
{
   return data.pos.y >= (windowHeight - 80) - data.rec.height;

}


AnimData updateAnimData(AnimData data, float dT, int maxFrame)
{
    data.runntingTime += dT;
    if (data.runntingTime >= data.updateTime)
    {
        data.runntingTime = 0.0;
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{

    //window dimensions
    int WindowDimensions [2];
    WindowDimensions [0] = 1280;
    WindowDimensions [1] = 720;

    const int windowWidth {WindowDimensions [0]};
    const int windowHeight {WindowDimensions [1]};

    InitWindow(WindowDimensions[0], WindowDimensions[1], "Glino");
        
    //Sounds
    InitAudioDevice();
    Sound Jump = LoadSound("Sounds/jump.wav");
    Sound melonPickUp = LoadSound("Sounds/pickupCoin.wav");
    Sound kill = LoadSound("Sounds/hitHurt.wav");
    Music PixelKing = LoadMusicStream("Sounds/Pixel Kings.wav");


    //Sound of countdown system 
    Sound countdownSound1 = LoadSound("Sounds/son1.wav");
    Sound countdownSound2 = LoadSound("Sounds/son2.wav"); 
    Sound countdownSound3 = LoadSound("Sounds/son3.wav"); 
    
    
    //Texture mountain
    Texture2D mountain =LoadTexture("textures/mountain.png");
    AnimData mountainData;
    mountainData.rec.width = mountain.width;
    mountainData.rec.height = mountain.height;
    mountainData.rec.x = 0;
    mountainData.rec.y = 0;
    mountainData.pos.x = 0;
    mountainData.pos.y = 0; 
    mountainData.frame = 0;
    mountainData.runntingTime = 0;
    mountainData.updateTime =0;
    
    
    //Texture treesBack
    Texture2D treesBack =LoadTexture("textures/treesBack.png");
    AnimData treesBackData;
    treesBackData.rec.width = treesBack.width;
    treesBackData.rec.height = treesBack.height;
    treesBackData.rec.x = 0;
    treesBackData.rec.y = 0;
    treesBackData.pos.x = 0;
    treesBackData.pos.y = (windowHeight - 80) - treesBack.height; 
    treesBackData.frame = 0;
    treesBackData.runntingTime = 0;
    treesBackData.updateTime =0;
    
    //Texture treesFront
    Texture2D treesFront =LoadTexture("textures/treesFront.png");
    AnimData treesFrontData;
    treesFrontData.rec.width = treesFront.width;
    treesFrontData.rec.height = treesFront.height;
    treesFrontData.rec.x = 0;
    treesFrontData.rec.y = 0;
    treesFrontData.pos.x = 0;
    treesFrontData.pos.y = (windowHeight - 80) - treesFront.height;
    treesFrontData.frame = 0;
    treesFrontData.runntingTime = 0;
    treesFrontData.updateTime =0;
    


    
    //Textures Dino
    Texture2D Dino = LoadTexture("textures/dino.png");
    AnimData dinoData;
    dinoData.rec.width = Dino.width/5;
    dinoData.rec.height = Dino.height;
    dinoData.rec.x = 0;
    dinoData.rec.y = 0;
    dinoData.pos.x = windowWidth/3 - dinoData.rec.width/2;
    dinoData.pos.y = ((windowHeight - 80) - dinoData.rec.height);
    dinoData.frame = 0;
    dinoData.runntingTime = 0;
    dinoData.updateTime = 0.1;

    
    //Texture Laeva
    Texture2D Laeva = LoadTexture("textures/LAEVA.png");
    const int NumOfLaevas{6};
    int LaevaDist{100};
            
    AnimData Laevas[NumOfLaevas];
    for (int i = 0; i < NumOfLaevas; i++)
    {
        Laevas[i].rec.x = 0.0;
        Laevas[i].rec.y = 0.0;
        Laevas[i].rec.width = Laeva.width/4;
        Laevas[i].rec.height = Laeva.height;
        Laevas[i].pos.x = windowWidth + LaevaDist;
        Laevas[i].pos.y = (windowHeight - 100) - Laeva.height;
        Laevas[i].frame = 0.0;
        Laevas[i].runntingTime = 0.0;
        Laevas[i].updateTime = 0.2;
        LaevaDist += 10000;
    }
    
    //Texture Watermelon
    Texture2D melon = LoadTexture("textures/Yellow Watermelon2.png");
    const int NumOfMelon{1};
    int MelonDist{1000};
            
    AnimData Melons[NumOfMelon];
    for (int i = 0; i < NumOfMelon; i++)
    {
        Melons[i].rec.x = 0.0;
        Melons[i].rec.y = 0.0;
        Melons[i].rec.width = melon.width;
        Melons[i].rec.height = melon.height;
        Melons[i].pos.x = windowWidth + MelonDist;
        Melons[i].pos.y = (windowHeight - 200) - melon.height;
        Melons[i].frame = 0.0;
        Melons[i].runntingTime = 0.0;
        Melons[i].updateTime = 0.2;
        MelonDist += 5000; 
    }



    //Texture Heart
    Texture2D heartTexture=LoadTexture("textures/heart.png");

    //====Variables of the game=====

    //Knight Is ducking
    bool DinoIsDuck{false};
    // Knight Velocity 
    int Dino_velocity{0};     
    // Knight Jump Hight 
    const int jump_hight{800};
    //Gravity (pixels/s/s)/frame
    const int gravity{2'000};
    //Objects Speed
    float ObjectSpeed{300};
    
    //Back speed 
    float mountainSpeed{15}; 
    float treesBackSpeed{70}; 
    float treesFrontSpeed{ObjectSpeed * 1.1f}; 
    
    //points
    int pointNum{0};
    float pointRunningTime{0};
    float pointUpdateTime {1};
    //Duck cooldown
    float DuckRunningTime{0};
    float DuckUpdateTime {0.45};
    //Collisions
    bool LaevaCollisions{false};

    //New system of lifes
    int lives{3};


    //System of invicibility
    bool isInvincibles{false};
    float invincibilityTimer{0.0};
    const float invincibilityDuration{5.0};
    int melonCount{0}; 
    int melonToInvincibles{0}; 
    
    //variable for visual blinking
    bool isFlashing{false};

    //SlowMotion Effect
    float timeScale{1.0};
    const float slowMotionFactor{0.2}; 
    float slowMotionTimer{0.0}; 
    const float slowMotionDuration{0.5}; 

    // New color for the invicibility effect
    const Color faint_white = {255, 255, 255, 128};

    //Menu
    bool Menu{true};
    bool MusicOn{true};

    //Variable for the countdown system
    bool isCountingDown{false};
    float countdownTimer{3.0f}; 
    const float countdownDuration{3.0f}; 
    int currentCountdownValue{3}; 
    bool musicStartedAfterCountdown{false}; 
    
    //story screen
    bool StoryScreen{false};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(PURPLE); 

    if(Menu)
    {

        //====Gestion of the first page=====

        ClearBackground(PURPLE); 

        // Title of the game
        DrawText("THE KNIGHT'S RACE", 
            windowWidth/2 - MeasureText("THE KNIGHT'S RACE", 50)/2, 50, 50, RED);

        // Academic information
        DrawText("C++ Project 2025", 20, windowHeight - 150, 20, RAYWHITE);
        DrawText(("Author: Mouhyadine Zakaria Djama"), 20, windowHeight - 120, 20, RAYWHITE);
        DrawText(("Teacher: Mohamed Hamid"), 20, windowHeight - 90, 20, RAYWHITE);

        // Small narration 
        DrawText("The Knight got lost in a parallel universe....", 
            windowWidth/2 - MeasureText("The Knight got lost in a parallel universe....", 30)/2, 200, 30, RED);
            
        // control option
        DrawText("CONTROLS", 
            850, 450, 40, RED);
        DrawText(">>SPACE<< TO JUMP OVER AXES", 
            800, 500, 20, BLUE);
        DrawText(">>S<< TO DUCK UNDER AXES", 
            800, 550, 20, BLUE);
        DrawText(">>Q<< TO TURN MUSIC OFF",
            800, 600, 20, BLUE);
        DrawText(">>E<< TO TURN MUSIC ON",
            800, 650, 20, BLUE);
            
        
        DrawText("ENTER TO START", 100, 400, 60, RED);
        
        // New optiob: See the history
        DrawText("S TO VIEW STORY & CAPABILITIES", 100, 480, 25, GREEN);
        
        // input gestion
        if (IsKeyPressed(KEY_ENTER))
        {
            Menu = false;
            isCountingDown = true; 
            countdownTimer = countdownDuration; 
            currentCountdownValue = 3;
            musicStartedAfterCountdown = false; 
            PlaySound(countdownSound3); 
        }
        
        // new input: go to the history
        if (IsKeyPressed(KEY_S))
        {
            Menu = false;
            StoryScreen = true;
        }

        // Music gestion
        if (IsKeyPressed(KEY_Q))
        {
            MusicOn = false;
        }
        if (IsKeyPressed(KEY_E) && !MusicOn)
        {
            MusicOn = true;
        }
    }
    else if(StoryScreen) //History screen gestion
    {
        ClearBackground(PURPLE); 
        
        // Title
        DrawText("THE STORY OF THE LOST KNIGHT", 
            windowWidth/2 - MeasureText("THE STORY OF THE LOST KNIGHT", 40)/2, 50, 40, YELLOW);
        
        // History
        DrawText("The brave but reckless Knight Glino went through a portal.", 
            50, 150, 25, RAYWHITE);
        DrawText("magical and found himself trapped in a world where dinosaurs", 
            50, 180, 25, RAYWHITE);
        DrawText("have been replaced by Flying Axes (Laeva).", 
            50, 210, 25, RAYWHITE);
        DrawText("His only hope: run and survive by avoiding danger!", 
            50, 240, 25, RAYWHITE);

        // Knight's special capacity
        DrawText("SPECIAL CAPACITY", 
            50, 350, 35, GREEN);
        DrawText("- JUMP (SPACE / W): Allows you to avoid high axes.", 
            70, 400, 25, BLUE);
        DrawText("- CROUCH (S): Allows you to pass under Flying Axes.", 
            70, 430, 25, BLUE);
        DrawText("- PAUSE/SLOW MOTION (DANGER ZONE): Approaching danger slows down time.", 
            70, 460, 25, BLUE);
        DrawText("- MELONS: Collecting 3 melons grants a short period of INVINCIBILITY.", 
            70, 490, 25, BLUE);
        DrawText("- HEARTS: Every 6th melon collected adds one life.", 
            70, 520, 25, BLUE);

        // come_back instructions
        DrawText("PRESS ENTER TO RETURN TO MENU", 
            windowWidth/2 - MeasureText("PRESS ENTER TO RETURN TO MENU", 30)/2, windowHeight - 100, 30, RED);

        // Return to the menu
        if (IsKeyPressed(KEY_ENTER))
        {
            StoryScreen = false;
            Menu = true;
        }
    }
    else if(isCountingDown) //gestion of the countdown
    {
        float dt = GetFrameTime();
        
        // update time
        countdownTimer -= dt;
        
        // Number and ssound changement logic
        int newCountdownValue = (int)ceil(countdownTimer);
        
        if (newCountdownValue < currentCountdownValue)
        {
            currentCountdownValue = newCountdownValue;
            
            if (currentCountdownValue == 2)
            {
                PlaySound(countdownSound2);
            }
            else if (currentCountdownValue == 1)
            {
                PlaySound(countdownSound1);
            }
        }
        
        // BACKGROUND DESIGN
        DrawTextureEx(mountain, {mountainData.pos.x, 0}, 0.0f, (float)windowHeight / mountain.height, WHITE);
        
        // COUNTDOWN DESIGN
        if (currentCountdownValue > 0)
        {
            
            std::string countdownText = std::to_string(currentCountdownValue);
            
            
            int textWidth = MeasureText(countdownText.c_str(), 100);
            int textX = (windowWidth / 2) - (textWidth / 2);
            int textY = (windowHeight / 2) - 50;
            
            DrawText(countdownText.c_str(), textX, textY, 100, YELLOW);
        }
        else //end of coutdown
        {
            isCountingDown = false; 
        }
        
    }
    else 
    {
        // New: Music starts playing at the beginning of the game
        if (MusicOn && !musicStartedAfterCountdown)
        {
            PlayMusicStream(PixelKing);
            musicStartedAfterCountdown = true;
        }

        if (LaevaCollisions)
        {
            
            DrawText("Game Over, Glino Died :(", windowWidth/2 - 200, 200, 60, RED);
            std::string points = "Score: ";
            DrawText("ENTER TO RESTART", windowWidth/2 - 200, 300, 60, RED);
            points.append(std::to_string(pointNum), 0, 999); 
            DrawText(points.c_str(), windowWidth/2 - 200, 400, 30, BLUE);

            // FINAL DISPLAY OF THE MELON COUNTER
            std::string finalMelons = "Melons Total: ";
            finalMelons.append(std::to_string(melonCount), 0, 999);
            DrawText(finalMelons.c_str(), windowWidth/2 - 200, 450, 30, GREEN);
            

            StopMusicStream(PixelKing);  
            DrawText("CONTROLS", 
            850, 450, 40, RED);
            DrawText(">>SPACE<< TO JUMP OVER AXES", 
            800, 500, 20, BLUE);
            DrawText(">>S<< TO DUCK UNDER AXES", 
            800, 550, 20, BLUE);
            DrawText(">>Q<< TO TURN MUSIC OFF", 
            800, 600, 20, BLUE);
            DrawText(">>E<< TO TURN MUSIC ON",
            800, 650, 20, BLUE);
            
            if (IsKeyPressed(KEY_ENTER))
            {
                LaevaCollisions = false;
                pointNum = 0;

                //Reinitialisation of lifes and melons
                lives=3;
                melonCount=0;
                melonToInvincibles=0;
                isInvincibles=false;
                invincibilityTimer=0.0;
                musicStartedAfterCountdown = false; 

                int LaevaDist = 100;
                for (int i = 0; i < NumOfLaevas; i++)
                {
                    Laevas[i].rec.x = 0.0;
                    Laevas[i].rec.y = 0.0;
                    Laevas[i].rec.width = Laeva.width/4;
                    Laevas[i].rec.height = Laeva.height;
                    Laevas[i].pos.x = windowWidth + LaevaDist;
                    Laevas[i].pos.y = (windowHeight - 100) - Laeva.height;
                    Laevas[i].frame = 0.0;
                    Laevas[i].runntingTime = 0.0;
                    Laevas[i].updateTime = 0.2;
                    LaevaDist += 10000;
                }                           
                
                for (int i = 0; i < NumOfMelon; i++)
                {
                Melons[i].pos.x = windowWidth + GetRandomValue(1000, 5000) ; 
                }
                ObjectSpeed = 300;
                mountainSpeed = 15;
                treesBackSpeed = 70;
                treesFrontSpeed = ObjectSpeed * 1.1f;
            }
        }
    
                
        else
        {
        if (dinoData.pos.y > (windowHeight - 80) - dinoData.rec.height)
        {
            dinoData.pos.y = (windowHeight - 80) - dinoData.rec.height;
        }

        //Music Logic
        
        if(IsMusicStreamPlaying(PixelKing)){
            UpdateMusicStream(PixelKing);
        }
        if(IsKeyPressed(KEY_Q) && MusicOn){
            MusicOn =false;
            StopMusicStream(PixelKing);
        }
        if(IsKeyPressed(KEY_E) && !MusicOn){
            MusicOn=true;
            musicStartedAfterCountdown = false; 
        }


        //Timer of Invicibility
        if(isInvincibles){
            invincibilityTimer+=GetFrameTime();
            if(invincibilityTimer>=invincibilityDuration){
                isInvincibles=false;
                isFlashing=false;
                invincibilityTimer=0.0;
            }
        }

        //Delta Time
        float dt{GetFrameTime()*timeScale};
        
                
        //Game Logic

        //Points Health
        pointRunningTime += dt;
        if (pointRunningTime >= pointUpdateTime)
        {   
        pointRunningTime = 0.0;
        pointNum++;
        if (ObjectSpeed<700)
        {
            ObjectSpeed+=10;
            mountainSpeed+=0.5f;
            treesBackSpeed+=2.5f;
            treesFrontSpeed = ObjectSpeed * 1.1f; 
        }
        
        }
        
        //Gestion of slowMotion
        if(timeScale<1.0){
            slowMotionTimer+=GetFrameTime();
            if(slowMotionTimer>=slowMotionDuration){
                timeScale=1.0; 
                slowMotionTimer= 0.0;
            }
        }
        
       


        // Ground check
        if (isOnGround(dinoData, windowHeight))
        {
            Dino_velocity = 0;
        }
        else{
            //Apply gravity
        Dino_velocity += gravity * dt;
        
        }
                
        for (AnimData Laeva :Laevas)
        {
            float Laevpad{30};
            Rectangle LaevRec {
                Laeva.pos.x + Laevpad,
                Laeva.pos.y + Laevpad,
                Laeva.rec.width - 2*Laevpad,
                Laeva.rec.height - 2*Laevpad
            };
            float DinoPad{30};            
            Rectangle dinoRec {
                dinoData.pos.x + DinoPad,
                dinoData.pos.y + DinoPad,
                dinoData.rec.width - 2*DinoPad,
                dinoData.rec.height - 2*DinoPad
            };


            //Danger rectangle to activate slow motion
            float dangerPad{15};
            Rectangle dangerRec{
                dinoData.pos.x+DinoPad-dangerPad,
                dinoData.pos.y+DinoPad-dangerPad,
                dinoData.rec.width-2*DinoPad+2*dangerPad,
                dinoData.rec.height-2*DinoPad+2*dangerPad,
            };
            
            if(CheckCollisionRecs(LaevRec,dangerRec)&& timeScale==1.0){
                if(!CheckCollisionRecs(LaevRec,dinoRec)){
                    timeScale=slowMotionFactor; //Activation of the slowMotion
                    slowMotionTimer= 0.0;
                }
            }

            //Gestion of real collisions
            if (CheckCollisionRecs(LaevRec, dinoRec) && !DinoIsDuck)
            {
                if(!isInvincibles){
                    lives--;
                    PlaySound(kill);

                    if(lives<=0){
                        LaevaCollisions=true;
                    }else{
                        isInvincibles= true;
                        invincibilityTimer=0.0;
                    }
                }
            }

            
        }
        //Duck
        if ((IsKeyPressed(KEY_S)))
        {
            DinoIsDuck = true;      
        }
        if (DinoIsDuck && DuckRunningTime < DuckUpdateTime)
        {
            DuckRunningTime += dt;

            dinoData.rec.x = 3*dinoData.rec.width;
        }
        else
        {
        DuckRunningTime = 0.0;
        DinoIsDuck =false;

        //update animData Dino
        dinoData = updateAnimData(dinoData, dt, 2);
        }
            
        //update AnimData Laeva
        for (int i = 0; i < NumOfLaevas; i++)
        {
            Laevas[i] = updateAnimData(Laevas[i], dt, 3);
        }
        
        //Jump
        if ((IsKeyPressed(KEY_SPACE) && isOnGround(dinoData, windowHeight))||(IsKeyPressed(KEY_W) && isOnGround(dinoData, windowHeight)))
        {
            Dino_velocity -= jump_hight;
            PlaySound(Jump);
        }
        //Collisions Check
        for (AnimData Melon :Melons)
        {
            float MelonPad{5};
            Rectangle MelonRec {
                Melon.pos.x + MelonPad,
                Melon.pos.y + MelonPad,
                Melon.rec.width - 2*MelonPad,
                Melon.rec.height - 2*MelonPad
            };
            float DinoPad{30};
            Rectangle dinoRec {
                dinoData.pos.x + DinoPad,
                dinoData.pos.y + DinoPad,
                dinoData.rec.width - 2*DinoPad,
                dinoData.rec.height - 2*DinoPad
            };
            
            
            if (CheckCollisionRecs(MelonRec, dinoRec))
            {
                PlaySound(melonPickUp);
                pointNum += 10;
                Melons[0].pos.x = windowWidth + GetRandomValue(1000, 5000); 

                melonCount++;
                melonToInvincibles++;

                if(melonCount%6==0){
                    lives++;
                }

                if(melonToInvincibles>=3){
                    isInvincibles=true;
                    isFlashing=true;
                    invincibilityTimer= 0.0;
                    melonToInvincibles=0;
                }
            }
        }
        
        
        //update position DINO
        dinoData.pos.y += Dino_velocity * dt;

        
        //Update position LAEVA
        for (int i = 0; i < NumOfLaevas; i++)
        {
            
            if (Laevas[i].pos.x <= -Laevas[i].rec.width) 
            {
                Laevas[i].pos.x = windowWidth + GetRandomValue(100,500); 
            }
            else{
            Laevas[i].pos.x -= (ObjectSpeed) * dt;
            }
        }

        //Update position Melon
        for (int i = 0; i < NumOfMelon; i++)
        {
            
            if (Melons[i].pos.x <= -Melons[i].rec.width) 
            {
                Melons[i].pos.x = windowWidth + GetRandomValue(1000, 5000) ;
            }
            else{
            Melons[i].pos.x -= (ObjectSpeed) * dt;
            }
        }

        
        // Logic of the parallax background
        
        
        mountainData.pos.x -= mountainSpeed * dt;
        if (mountainData.pos.x <= -mountain.width)
        {
            mountainData.pos.x = 0; 
        }


        treesBackData.pos.x -= treesBackSpeed * dt;
        if (treesBackData.pos.x <= -treesBack.width)
        {
            treesBackData.pos.x = 0; 
        }
        
        
        treesFrontData.pos.x -= treesFrontSpeed * dt;
        if (treesFrontData.pos.x <= -treesFront.width)
        {
            treesFrontData.pos.x = 0; 
        }
        
        
        // DECOR DESIGN
        
        // 1. Sky layer/background (mountain) 
        DrawTextureEx(mountain, {mountainData.pos.x, 0}, 0.0f, (float)windowHeight / mountain.height, WHITE);
        DrawTextureEx(mountain, {mountainData.pos.x + mountain.width * ((float)windowHeight / mountain.height), 0}, 0.0f, (float)windowHeight / mountain.height, WHITE);
        
        //2. Middle layer (treesBack)
        DrawTextureRec(treesBack, treesBackData.rec, treesBackData.pos, WHITE);
        DrawTextureRec(treesBack, treesBackData.rec, {treesBackData.pos.x + treesBack.width, treesBackData.pos.y}, WHITE);

        // 3. Closest layer (treesFront)
        DrawTextureRec(treesFront, treesFrontData.rec, treesFrontData.pos, WHITE);
        DrawTextureRec(treesFront, treesFrontData.rec, {treesFrontData.pos.x + treesFront.width, treesFrontData.pos.y}, WHITE);

        
        // Design of game elements (Dino, Laeva, Melon)
        DrawTextureRec(Dino, dinoData.rec, dinoData.pos, (isFlashing && (((int) invincibilityTimer*10)%2==0))? faint_white:WHITE);

        for (int i = 0; i < NumOfLaevas; i++)
        {
            DrawTextureRec(Laeva, Laevas[i].rec, Laevas[i].pos, WHITE);
        }
        for (int i = 0; i < NumOfMelon; i++)
        {
            DrawTextureRec(melon, Melons[i].rec, Melons[i].pos, WHITE);
        }
        
        
        // Drawing of the UI interface

        // Hearts display
        int heartXStart = windowWidth - 150; 
        int heartY = 20; 
        int heartPadding = 5; 
        float scale = 1.0f;

        for(int i=0;i<lives;i++){
            DrawTextureEx(heartTexture,{(float)(heartXStart+i*(heartTexture.width+heartPadding)), (float)heartY},0.0f,scale,WHITE);
        }

        // Score display
        std::string points = "Score: ";
        points.append(std::to_string(pointNum), 0, 999);
        DrawText(points.c_str(), 50, 20, 30, RED);     

        // total melon comptor (melonCount)
        std::string melonCounter = "Melons: ";
        melonCounter.append(std::to_string(melonCount), 0, 999);
        DrawText(melonCounter.c_str(), 50, 50, 30, GREEN); 

        //Invisibility comptor(melonToInvincibles)
        std::string invincibilityStatus = "Invincibilite: ";
        invincibilityStatus.append(std::to_string(melonToInvincibles));
        invincibilityStatus.append("/3");
        DrawText(invincibilityStatus.c_str(), 50, 90, 20, BLUE); 

    }
    }
        
            

        EndDrawing();
        
        
        
        
    }

    //Unload of all textures
    UnloadTexture(Dino);
    UnloadTexture(Laeva);
    UnloadTexture(melon);
    UnloadTexture(heartTexture);
    UnloadTexture(mountain);
    
    UnloadTexture(treesBack);
    UnloadTexture(treesFront);
    UnloadSound(countdownSound1);
    UnloadSound(countdownSound2);
    UnloadSound(countdownSound3);
    CloseWindow();


}