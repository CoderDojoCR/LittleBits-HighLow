int highPin = 0;
int lowPin = A1;
int numberPin = 5;
int buzzerPin = 9;

int randomValue;
int guessHigherCounter = 0;       // how many times we have seen high button
int guessLowerCounter = 0;        // how many times we have seen low button
int guessHigher;           // the current value read from the top button
int guessLower;            // the current value read from the bottom button
int current_state = LOW;    // the debounced input value

// the following variable is a long because the time, measured in milliseconds,
// will quickly become a bigger number than can be stored in an int.
long time = 0;         // the last time the output pin was sampled
int debounce_count = 50; // number of millis/samples to consider before declaring a debounced input

void setup() {
  // put your setup code here, to run once:
  pinMode(highPin, INPUT);
  pinMode(lowPin, INPUT);
  pinMode(numberPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // seed the random number generator from a random value on an unconnected pin
  randomSeed(analogRead(A2));
  randomValue = random(0, 255);
  analogWrite(numberPin, randomValue);
}

void loop() {
  
  if(millis() != time)
  {
    guessHigher = digitalRead(highPin);
    guessLower = digitalRead(lowPin);

    // decrement counters to 0 if we see LOW on inputs
    if (guessHigher == current_state && guessHigherCounter > 0)
    {
      guessHigherCounter--;
    }
    if (guessLower == current_state && guessLowerCounter > 0)
    {
       guessLowerCounter--; 
    }
    
    // increment counters if we see HIGH (!LOW) on inputs
    if(guessHigher != current_state)
    {
       guessHigherCounter++; 
    }
    if (guessLower != current_state)
    {
      guessLowerCounter++;  
    }
    
    // If the Input has shown the same value for long enough let's switch it
    if(guessHigherCounter >= debounce_count)
    {
      guessHigherCounter = 0;
      current_state = guessHigher;
      
      if (guessHigher == HIGH) 
      {
        // the guess was high
        doGuess(true);  
      }
    }
    else if (guessLowerCounter >= debounce_count)
    {
      guessLowerCounter = 0;
      current_state = guessLower;
        
      if (guessLower == HIGH) 
      {
        doGuess(false);
      }
    }
    
    time = millis();
  }
  
}

void doGuess(boolean high)
{
  int newRandomValue = random(0, 255);
  analogWrite(numberPin, newRandomValue);
  
  if ((newRandomValue < randomValue && !high) || 
      (newRandomValue > randomValue && high))
  {
    // new value was higher and we guessed higher, or
    // new value was lower and we guessed lower
    // this means we guessed correctly!
    buzzRight();
  }
  else {
    // we guessed incorrectly
    buzzWrong();
  }
  
  randomValue = newRandomValue;
}

void buzzRight()
{
  analogWrite(buzzerPin, 255);
  delay(200);
  analogWrite(buzzerPin, 0); 
}

void buzzWrong()
{
  analogWrite(buzzerPin, 255);
  delay(200);
  analogWrite(buzzerPin, 0); 
  delay(200);
  analogWrite(buzzerPin, 255);
  delay(200);
  analogWrite(buzzerPin, 0);
}
