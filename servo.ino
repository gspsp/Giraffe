void servoSetup(){
    servo.attach(13, 500, 2500);
    servo.write(conf.get("angle").toInt());
}