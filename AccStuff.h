

// --- Acelerometer Configuration ---

LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A
float aX, aY, aZ, gX, gY, gZ;
const float accelerationThreshold = 2.5; // threshold of significant in G's
const int numSamples = 119;
int samplesRead = numSamples;
#define GYRO_ENABLE_PIN PD5


void enableAcc(){
    //gyro enable pinMode
  pinMode(GYRO_ENABLE_PIN,OUTPUT);
  digitalWrite(GYRO_ENABLE_PIN,HIGH);
}