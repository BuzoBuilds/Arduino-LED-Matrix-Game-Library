//class that controlls thr frame rate of the game
class FrameRateController{
    public: 
        long frameRate;   
        //initalizes the frame rate controller with the desired framerate
        void initFrameRateController(long frameRate){
            this->frameRate = frameRate; 
            this->currFrameStartTime = 0;  
            this->timeForEachFrame = (long)((1/frameRate) * 1000); // in milis 

        } 
        //should be called at the beginning of each frame
        void setFrameStartTime(){
            currFrameStartTime = millis(); 
        } 

        //should be called at the end of each frame
        void waitTillNextFrame(){
            long elaspedTime = millis() - currFrameStartTime;  
            long timeRemaining = timeForEachFrame - elaspedTime; 
            if(timeRemaining > 0){
                delay(timeRemaining); 
            }


        }

    private:
        long timeForEachFrame; // in milis  
        long currFrameStartTime; 

};  

//reprsents a colider for a gameobject
class Colider{
    public: 
        float x; 
        float y; 
        float l; 
        float w;

        Colider * next; 
};  


//base class that represents any object that is within the game. The player, the enemies, etc...
class GameObject{
    public: 
        float x; 
        float y; 
        Colider * head;  //linked list pof coliders

        //moves gameobject by dx and dy
        void translate(float dx, float dy){ 
            if(x +dx < 127 && x+dx >-128){ 
                 x += dx; 

                //move coliders
                Colider * curr = head; 
                while(curr != NULL){
                    curr->x +=dx; 
                    curr = curr->next; 
                }
            } 
            
            if(y + dy < 127 && y +dy > -128){
                y += dy; 

                //move coliders
                Colider * curr = head; 
                while(curr != NULL){ 
                    curr->y += dy; 
                    curr = curr->next; 
                }
            }
            
               

        } 

        //draws the gameobject by adding content into the LED bitmap 
        //should be overwritten
        void draw(char state){
            //abstract function that will be implemented by deriving class
        } 

        //detects wether or not the current gameobject colided with the gameobject o
        bool didItColideWithGO(GameObject * o){
            Colider * curr = head; 
            while(curr != NULL){
                Colider * curr2 = o->head; 
                while(curr2 != NULL){
                    //if both square coliders intercept then return true;  
                    if(doColidersOverlap(curr, curr2)){
                        return true;
                    }
                    curr2 = curr2->next; 
                } 
                curr = curr->next; 
            } 

            return false; 
        } 

        //detects which walls (x=0,x=63.y=0,y=31) that the current gameobject is coliding withv
        //0b0001 -> coliding with left wall 
        //0b0010 -> coliding with top wall 
        //0b0100 -> coliding with right wall
        //0b1000 -> coliding wiht bottom wall 
        //if its coliding with more than 1 wall the appropiate bits will be 1
        char didItColideWithWall(){ 
            char wallsColidedWith = 0; 
            Colider * curr = head; 
            while(curr != NULL){
                if(curr->x < 0){
                     wallsColidedWith |= 0b00000001;
                } 
                if((curr->y + curr->l) > 31){
                    wallsColidedWith |= 0b00000010;
                } 
                if((curr->x + curr->w) > 63){
                    wallsColidedWith |= 0b00000100;
                } 
                if(curr->y < 0){
                    wallsColidedWith |= 0b00001000; 
                } 

                curr = curr->next; 
            } 

            return wallsColidedWith; 
        }  

        //adds a rectangular colider to the gameobject
        void addColider(Colider * c){ 
            if(head == NULL){
                head = c; 
                return; 
            }
            Colider * curr = head; 
            while(curr->next != NULL){
                curr = curr->next;
            }  
            curr->next = c; 

        } 

        //any last words, gameobject...
        void destroyGameObject(){ 
            //free coliders
            Colider * rabbit = head;
            Colider * turtle = NULL; 

            while(rabbit->next != NULL){ 
                turtle = rabbit; 
                rabbit = rabbit->next; 
                free(turtle);
            } 

            free(rabbit); 
            //free gameobjects
            free(this); 
        }

    private: 
        bool doColidersOverlap(Colider * c1, Colider * c2){
            if(((c1->x + c1->w) < c2->x) || ((c2->x + c2->w) < c1->x)){  
                return false; 
            } 

            if((c1->y > (c2->y + c2->l)) || (c2->y > (c1->y + c1->l))){   
                return false;
            } 

            return true; 
        }
}; 

//represents the controller
class Controller{ 
    int controllerInterruptPin; 
    int upPin; 
    int downPin;
    int leftPin; 
    int rightPin; 
    int APin; 
    int BPin;   

    public:
        volatile bool buttonPressed; 
        volatile bool isInputProcessed;    
        volatile char input; 
    

    
      
        //initalizes controller object with the pin parameters
        void initController(int controllerInterruptPin, int upPin, int downPin, int leftPin, int rightPin, int APin, int BPin ){
            this->controllerInterruptPin = controllerInterruptPin;  
            pinMode(this->controllerInterruptPin, INPUT); //maybe input_pullup 

            this->upPin = upPin;  
            pinMode(this->upPin, INPUT); 

            this->downPin = downPin;  
            pinMode(this->downPin,INPUT); 

            this->leftPin = leftPin; 
            pinMode(this->leftPin, INPUT); 

            this->rightPin = rightPin;  
            pinMode(this->rightPin, INPUT); 

            this->APin = APin; 
            pinMode(this->APin, INPUT);  

            this->BPin = BPin;  
            pinMode(this->BPin, INPUT); 

            buttonPressed = 0;  
            isInputProcessed = 0; 

        } 

        
        //should be called when an interrupt has occured due to the controller
        void controllerInterruptHandeler(){   
            Serial.println("***Interrupted***"); 
            if(digitalRead(controllerInterruptPin) == HIGH){ 
                long interruptBufferInterval = 100;  
                static long lastInterruptTime = 0;  
                long currInterruptTime = millis(); 
        
                
                         
                        if(currInterruptTime - lastInterruptTime >= interruptBufferInterval){ 
                            lastInterruptTime = currInterruptTime;
                        //if sufficent time has passed since last interrupt probably not a bounce
                            Serial.println("button pressed"); 
                            buttonPressed = 1;  
                            isInputProcessed = 0;   
                            if(digitalRead(upPin)==HIGH){
                                input |= 0b00000001;
                            } 
                            if(digitalRead(rightPin) == HIGH){
                                input |= 0b00000010;
                            }  
                            if(digitalRead(downPin)==HIGH){
                                input |= 0b00000100;
                            } 
                            if(digitalRead(leftPin) == HIGH){
                                input |= 0b00001000;
                            }  
                            if(digitalRead(BPin) == HIGH){
                                input |= 0b00100000;
                            }  
                            if(digitalRead(APin) == HIGH){ 
                                input |= 0b00010000;
                            }  
                        }

                            //buttonPressed = 1; 
                    }
                else{  
                    Serial.print("button released"); 
                    if(isInputProcessed == 1){
                        input = 0; 
                    }  
                    buttonPressed = 0; 
                } 

            }  

        //should be called after the portion of the frame that the input is processed
        void inputProcessed(){ 
            if(buttonPressed==0){
                input = 0; 
            }
            else{
            input &= 0b11001111; 
            } 
            isInputProcessed =1; 
        }

}; 

//utilities 

 void charToBinArray(char n, bool * Bits ){
                for(int i = 0; i < 8; i++){
                    Bits[7-i] = n%2;
                    n = n/2; 
                }  
                
                
}  

