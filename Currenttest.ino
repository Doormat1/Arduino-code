 // put your setup code here, to run once:
Serial.begin (9600);
 pinMode (currentin, INPUT);
  pinMode (currentout, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
currentin =analogRead(current1);
currentout =analogRead(current2);
Serial.print ("Current in ");
Serial.println(currentin);
Serial.print ("Current out ");
Serial.println(currentout);
delay (500);
}
