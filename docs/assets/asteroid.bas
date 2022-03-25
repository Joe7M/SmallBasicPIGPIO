'Asteroid for SmallBASIC PiGPIO
'SmallBASIC 12.23 Raspberry Pi
'Joerg Siebenmorgen
'MIT License, 2022
'https://joe7m.github.io/SmallBasicPIGPIO/sbconsole.html
'
'sudo sbasic -m /home/pi/SmallBasicPIGPIO/bin asteroids.bas

import SmallBasicPIGPIO as gpio

const OLED_X = 128
const OLED_Y = 64 

Button_Up = CreatePushButton(20)
Button_Down = CreatePushButton(26)
Button_Left = CreatePushButton(21)
Button_Right = CreatePushBUtton(16)
Button_A = CreatePushButton(19)
Button_B = CreatePushButton(13)

Ship  = [[0,-1], [0.7,1],  [0,0.6], [-0.7,1], [0,-1]]
Bullet = [[0,-1], [0.4,1], [-0.4,1], [0,-1]]
Asteroid1 = [[1,0.6], [1,0.2], [1,-0.6], [0.6,-1], [-0.6,-1], [-1,-0.4], [-1, 0.6], [-0.4,1], [1,0.6]]

BulletsMax = 100
BulletsLast = 0
BulletsVel = 0.03
dim Bullets(BulletsMax - 1)

AsteroidsMax = 50
AsteroidsLast = 0
AsteroidsActive = 0
dim Asteroids(AsteroidsMax - 1)

Player.x = 64
Player.y = 32
Player.vel = [0,0]
Player.acc = 0.00002
Player.alpha = 0
Player.lives = 3
Player.points = 0

Level = 1

GAMESTATE_LEVEL_IS_RUNNING = 1
GAMESTATE_PLAYER_IS_DEAD = 2
GAMESTATE_MENUE = 3
GAMESTATE_NEXT_LEVEL = 4
GAMESTATE_GAME_OVER = -1
GAMESTATE_QUIT_GAME = -2
GameState = GAMESTATE_MENUE

gpio.OLED1_Open()

randomize

while(GameState > GAMESTATE_QUIT_GAME)

    DoMenu()
    DoGameLoop()

wend

gpio.OLED1_Close()


'#############################################
sub DoGameLoop()

    ' Reset the player
    Player.x = 64
    Player.y = 32
    Player.lives = 3
    Player.points = 0
    Player.vel = [0,0]
    Player.alpha = 0
    Level = 1

   
    ' Create some asteroids for the first level
    ClearAsteroids()
    for ii = 1 to Level
        AddAsteroid(RndMinMax(0,OLED_X) ,RndMinMax(0,OLED_Y), [RndMinMax(-0.01, 0.01), RndMinMax(-0.01, 0.01)], RndMinMax(7,12))
    next
    
    
    ' Enter the actual game loop    
    while(GameState > GAMESTATE_GAME_OVER)
    
        while(GameState == GAMESTATE_LEVEL_IS_RUNNING)
            
            
            InvincibleCountDown = 0
            Player.x = OLED_X/2
            Player.y = OLED_Y/2
            Player.vel = [0,0]
        
            while(GameState == GAMESTATE_LEVEL_IS_RUNNING)
                
                TickStart = ticks()
                
                gpio.OLED1_Cls()
                
                UpdateKeys()
                
                UpdatePlayer()
                UpdateBullets()
                UpdateAsteroids()
                
                if(InvincibleCountDown < 3000)
                    InvincibleCountDown = InvincibleCountDown + DeltaTime
                    gpio.OLED1_Circle(Player.x, Player.y, 7)
                else
                    CheckCollisionPlayer()
                end if
                
                CheckCollisionBullets()
                    
                DrawShip()
                DrawBullets()
                DrawAsteroids()
                
                if(AsteroidsActive <= 0)
                    GameState = GAMESTATE_NEXT_LEVEL
                    Level++
                end if
                
                gpio.OLED1_At(0,0)
                gpio.OLED1_Print("S: " + str(Player.lives) + " P: " + str(Player.points))
                
                gpio.OLED1_Display()

                TickEnd = ticks()
                DeltaTime2 = TickEnd - TickStart
                DeltaTime = DeltaTime2
                
                if(DeltaTime < 20)
                    delay(20 - DeltaTime)
                    DeltaTime = 20
                end if
            wend
            
            
            ' Player died, show explosion
            Explosion = 0
            ExplosionSize = 0
                
            while(GameState == GAMESTATE_PLAYER_IS_DEAD)
                
                TickStart = ticks()
                
                gpio.OLED1_Cls()
                
                UpdateBullets()
                UpdateAsteroids()
                
                CheckCollisionBullets()
                    
                DrawBullets()
                DrawAsteroids()
                
                if(Explosion < 2000)
                    Explosion = Explosion + DeltaTime
                    ExplosionSize = ExplosionSize + 0.02 * DeltaTime
                    gpio.OLED1_Circle(Player.x, Player.y, ExplosionSize,1,1)
                else
                    GameState = GAMESTATE_LEVEL_IS_RUNNING
                    Player.vel = [0,0]                    
                end if
                
                gpio.OLED1_At(0,0)
                gpio.OLED1_Print("S: " + str(Player.lives) + " P: " + str(Player.points))
                
                gpio.OLED1_Display()

                TickEnd = ticks()
                DeltaTime2 = TickEnd - TickStart
                DeltaTime = DeltaTime2
                
                if(DeltaTime < 20)
                    delay(20 - DeltaTime)
                    DeltaTime = 20
                end if
            wend
            
            'Check players lives. If no live left, then we go to the menu
            if(Player.lives == 0)
                GameState = GAMESTATE_GAME_OVER
            end if
                        
        wend
              
        
        ' Next Level Countdown
        NextLevelCounter = 0
        ClearAsteroids()
        for ii = 1 to Level
            AddAsteroid(RndMinMax(0,OLED_X) ,RndMinMax(0,OLED_Y), [RndMinMax(-0.01, 0.01), RndMinMax(-0.01, 0.01)], RndMinMax(7,12))
        next

        while(GameState == GAMESTATE_NEXT_LEVEL)
            gpio.OLED1_Cls()
           
            TickStart = ticks()
            
            UpdateBullets()
                                
            UpdateAsteroids()
            DrawAsteroids()
            
            if(NextLevelCounter < 5000)
                NextLevelCounter = NextLevelCounter + DeltaTime
            else
                GameState = GAMESTATE_LEVEL_IS_RUNNING                    
            end if
            
            gpio.OLED1_At(40,28)
            gpio.OLED1_Print("LEVEL " + Level)   
            gpio.OLED1_At(62,38)           
            gpio.OLED1_Print(str(5 - floor(NextLevelCounter / 1000)))
            
            gpio.OLED1_At(0,0)
            gpio.OLED1_Print("S: " + str(Player.lives) + " P: " + str(Player.points))
                    
            gpio.OLED1_Display()

            TickEnd = ticks()
            DeltaTime2 = TickEnd - TickStart
            DeltaTime = DeltaTime2
            
            if(DeltaTime < 20)
                delay(20 - DeltaTime)
                DeltaTime = 20
            end if
        wend

    wend
end sub



sub DoMenu()

    TextBlink = 0
    ClearAsteroids()
    AddAsteroid(RndMinMax(0,OLED_X) ,RndMinMax(0,OLED_Y), [RndMinMax(-0.01, 0.01), RndMinMax(-0.01, 0.01)], 2)
    AddAsteroid(RndMinMax(0,OLED_X) ,RndMinMax(0,OLED_Y), [RndMinMax(-0.01, 0.01), RndMinMax(-0.01, 0.01)], 4)
    AddAsteroid(RndMinMax(0,OLED_X) ,RndMinMax(0,OLED_Y), [RndMinMax(-0.01, 0.01), RndMinMax(-0.01, 0.01)], 6)
    AddAsteroid(RndMinMax(0,OLED_X) ,RndMinMax(0,OLED_Y), [RndMinMax(-0.01, 0.01), RndMinMax(-0.01, 0.01)], 10)
    'AddAsteroid(RndMinMax(0,OLED_X) ,RndMinMax(0,OLED_Y), [RndMinMax(-0.01, 0.01), RndMinMax(-0.01, 0.01)], RndMinMax(7,10))
    'AddAsteroid(RndMinMax(0,OLED_X) ,RndMinMax(0,OLED_Y), [RndMinMax(-0.01, 0.01), RndMinMax(-0.01, 0.01)], RndMinMax(7,10))
    GameState = GAMESTATE_MENUE
               
    while(GameState == GAMESTATE_MENUE)
        
        TickStart = ticks()

        gpio.OLED1_Cls()

        UpdateKeys()
        
        UpdateAsteroids()
        DrawAsteroids()        
        
        gpio.OLED1_At(18,10)
        gpio.OLED1_SetTextSize(16)
        gpio.OLED1_Print("ASTEROID")
        gpio.OLED1_SetTextSize(8)
        gpio.OLED1_At(38,28)
        gpio.OLED1_Print("SmallBASIC")
        gpio.OLED1_At(52,38)
        gpio.OLED1_Print("PiGPIO")
               
        
        TextBlink = iff(TextBlink < 150, TextBlink + DeltaTime, 0)
        if(TextBlink < 75)
            gpio.OLED1_At(38,50)
            gpio.OLED1_Print("Press START")
        end if

        gpio.OLED1_Display()

        TickEnd = ticks()
        DeltaTime2 = TickEnd - TickStart
        DeltaTime = DeltaTime2
        
        if(DeltaTime < 20)
            delay(20 - DeltaTime)
            DeltaTime = 20
        end if

    wend
end sub


func CreatePushButton(Pin)
    local B
    B.Pin = Pin
    B.DebounceTimer = 0
    B.LastState = false
    
    gpio.GPIO_SetInput(Pin)
    
    CreatePushButton = B
end func

sub UpdateKeys()
    
    select case GameState
        case GAMESTATE_MENUE:
            
            if(ButtonPressed(Button_A)) then
                GameState = GAMESTATE_LEVEL_IS_RUNNING
            end if
            if(ButtonPressedAgain(Button_B)) then
                GameState = GAMESTATE_QUIT_GAME
            endif
            
    
        case GAMESTATE_LEVEL_IS_RUNNING
            if(ButtonPressed(Button_Up)) then
                PlayerFaster()
            endif
            if(ButtonPressed(Button_Down)) then
                PlayerSlower()
            endif
            if(ButtonPressed(Button_Left)) then
                PlayerRotateLeft()
            endif
            if(ButtonPressed(Button_Right)) then
                PlayerRotateRight()
            endif
            if(ButtonPressed(Button_A)) then
                AddBullet()
            endif
            if(ButtonPressedAgain(Button_B)) then
                GameState = GAMESTATE_GAME_OVER
            endif
            
    end select


end sub

func ButtonPressed(byref B)
    
    if(B.DebounceTimer > 10) then
        state = 1 - gpio.GPIO_READ(B.Pin)
        
        if(B.LastState != state) then
            B.DebounceTimer = 0
            B.LastState = state
        end if
    else
        B.DebounceTimer = B.DebounceTimer + DeltaTime
        state = B.LastState
    end if
    
    ButtonPressed = state
    
end func

func ButtonPressedAgain(byref B)
    local ReturnVal

    ReturnVal = 0
    
    if(B.DebounceTimer > 10) then
        state = 1 - gpio.GPIO_READ(B.Pin)
                             
        if(B.LastState != state) then
            B.DebounceTimer = 0
            B.LastState = state
            ReturnVal = state            
        end if        
    else
        B.DebounceTimer = B.DebounceTimer + DeltaTime
    end if
    
    ButtonPressedAgain = ReturnVal
    
end func



func RNDMinMax(MinValue, MaxValue)
    RNDMinMax = rnd * (MaxValue - MinValue) + MinValue
end func

sub CheckCollisionBullets()
    local bb,aa, Radius, distance, size

    for bb = 0 to BulletsMax - 1
        
        if(Bullets[bb].active) then
        
            for aa = 0 to AsteroidsMax - 1
            
                if(Asteroids[aa].active) then
                    distance = abs((Bullets[bb].x - Asteroids[aa].x)^2 + (Bullets[bb].y - Asteroids[aa].y)^2)
                    
                    if(distance < (1.8*(Asteroids[aa].size))^2)
                        Player.points++
                        Bullets[bb].active = false
                        Asteroids[aa].active = false
                        AsteroidsActive--
                        size = Asteroids[aa].size / 2
                        if(size > 1)
                            AddAsteroid(Asteroids[aa].x , Asteroids[aa].y, [Asteroids[aa].vel[0] + RndMinMAx(-0.01,0.01), Asteroids[aa].vel[0] + RndMinMAx(-0.01,0.01)], size)
                            AddAsteroid(Asteroids[aa].x , Asteroids[aa].y, [Asteroids[aa].vel[0] + RndMinMAx(-0.01,0.01), Asteroids[aa].vel[0] + RndMinMAx(-0.01,0.01)], size)
                        end if
                        
                    end if
                end if
            
            next
        end if
    next
end sub


sub CheckCollisionPlayer()
    local ii, Radius, distance

    for ii = 0 to AsteroidsMax - 1
    
        if(Asteroids[ii].active)
                    
            distance = abs((Player.x - Asteroids[ii].x)^2 + (Player.y - Asteroids[ii].y)^2)
            
            if(distance < (1.9*Asteroids[ii].size)^2)
                Player.lives--
                GameState = GAMESTATE_PLAYER_IS_DEAD
                'Asteroids[ii].active = false
            end if
        end if
    
    next

end sub

sub OLEDDrawpoly(byref P)

    local l,ii
    
    l = ubound(P) - 1
    
    for ii = 0 to l
        gpio.OLED1_Line(P[ii][0], P[ii][1], P[ii+1][0], P[ii+1][1])
    next

    gpio.OLED1_Line(P[l+1][0], P[l+1][1], P[0][0], P[0][1])

end sub

sub DrawBullets()
    local ii, M
    
    for ii = 0 to BulletsMax - 1
    
        if(Bullets[ii].active) then
            Dim M(2,2)
            M3Ident M
            M3Trans M, Bullets[ii].x, Bullets[ii].y
            M3Scale M, 0,0,1.4,1.4
            M3Rotate M, Bullets[ii].alpha
            BulletT = Bullet
            M3Apply M, BulletT
            
            OLEDDrawpoly(BulletT)
        end if
    next
    
end sub

sub DrawAsteroids()
    local ii, M
    
    for ii = 0 to AsteroidsMax - 1
    
        if(Asteroids[ii].active) then
            Dim M(2,2)
            M3Ident M
            M3Trans M, Asteroids[ii].x, Asteroids[ii].y
            M3Scale M, 0,0,Asteroids[ii].size,Asteroids[ii].size
            M3Rotate M, Asteroids[ii].alpha
            
            AsteroidT = Asteroid1
            
            M3Apply M, AsteroidT
            
            OLEDdrawpoly(AsteroidT)
        end if
    next
    
end sub


sub DrawShip()
    local M
    Dim M(2,2)
    
    M3Ident M
    M3Trans M, Player.x, Player.y
    M3Scale M, 0,0,4,4
    M3Rotate M, Player.alpha
    ShipT = Ship
    M3Apply M, ShipT

    OLEDDrawPoly(ShipT)
    
end sub

sub UpdateBullets
    local ii
    
    for ii = 0 to BulletsMax - 1
        if(Bullets[ii].active)
            Bullets[ii].x = Bullets[ii].x + BulletsVel * DeltaTime * sin(Bullets[ii].alpha)
            Bullets[ii].y = Bullets[ii].y - BulletsVel * DeltaTime * cos(Bullets[ii].alpha)
            
            if(Bullets[ii].x < 0 or Bullets[ii].x > OLED_X) then
                Bullets[ii].active = false
            else if(Bullets[ii].y < 0 or Bullets[ii].y > OLED_Y) then
                Bullets[ii].active = false
            end if
                        
        end if
    next

end sub

sub UpdateAsteroids
    local ii
    
    for ii = 0 to AsteroidsMax - 1
        if(Asteroids[ii].active) then
        
            Asteroids[ii].x = Asteroids[ii].x + Asteroids[ii].vel[0] * DeltaTime
            Asteroids[ii].y = Asteroids[ii].y - Asteroids[ii].vel[1] * DeltaTime
            Asteroids[ii].alpha = Asteroids[ii].alpha + Asteroids[ii].rotvel * DeltaTime
            
            if(Asteroids[ii].x < 0) then
                Asteroids[ii].x = OLED_X
            else if(Asteroids[ii].x > OLED_X) then
                Asteroids[ii].x = 0
            end if
            if(Asteroids[ii].y < 0) then
                Asteroids[ii].y = OLED_Y
            else if(Asteroids[ii].y > OLED_Y) then
                Asteroids[ii].y = 0
            end if
                                
        end if
    next

end sub

sub AddBullet()
    
    if(BulletsLast == BulletsMax) then
        BulletsLast = 0
    end if
    
    Bullets[BulletsLast].active = true
    Bullets[BulletsLast].x = Player.x
    Bullets[BulletsLast].y = Player.y
    Bullets[BulletsLast].alpha = Player.alpha
    
    BulletsLast++

end sub

sub AddAsteroid(x, y ,vel, size)
    
    if(AsteroidsLast == AsteroidsMax) then
        AsteroidsLast = 0
    end if
    
    Asteroids[AsteroidsLast].active = true
    Asteroids[AsteroidsLast].x = x
    Asteroids[AsteroidsLast].y = y
    Asteroids[AsteroidsLast].alpha = rnd * 2 * pi
    Asteroids[AsteroidsLast].rotvel = RndMinMax(-0.005, 0.005)
    Asteroids[AsteroidsLast].vel = vel
    Asteroids[AsteroidsLast].size = size
    
    AsteroidsLast++
    AsteroidsActive++

end sub

sub ClearAsteroids()
    local ii
    
    AsteroidsLast = 0
    AsteroidsActive = 0
    
    for ii = 0 to AsteroidsMax - 1 
        Asteroids[ii].active = false
    next
    
end sub

sub UpdatePlayer()
    local M
    
    Player.x = Player.x + Player.vel[0] * DeltaTime
    Player.y = Player.y - Player.vel[1] * DeltaTime
    
    if(Player.x < 0) then
        Player.x = OLED_X
    else if(Player.x > OLED_X) then
        Player.x = 0
    end if
    if(Player.y < 0) then
        Player.y = OLED_Y
    else if(Player.y > OLED_Y) then
        Player.y = 0
    end if
    
end sub

sub PlayerRotateLeft
    Player.alpha = Player.alpha - 0.003 * DeltaTime
    Player.alpha = iff(Player.alpha < 0, 2*pi, Player.alpha) 
end sub

sub PlayerRotateRight
    Player.alpha = Player.alpha + 0.003 * DeltaTime
    Player.alpha = iff(Player.alpha > 2 * pi, 0, Player.alpha) 
end sub

sub PlayerFaster
    Player.vel[0] = Player.vel[0] + Player.acc * DeltaTime *  sin(Player.alpha)
    Player.vel[1] = Player.vel[1] + Player.acc * DeltaTime *  cos(Player.alpha)
end sub

sub PlayerSlower
    Player.vel[0] = Player.vel[0] - Player.acc * DeltaTime *  sin(Player.alpha)
    Player.vel[1] = Player.vel[1] - Player.acc * DeltaTime *  cos(Player.alpha)
end sub
