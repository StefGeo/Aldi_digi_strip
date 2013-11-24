//StripInvader (c) 2011 Michael Vogt <michu@neophob.com> // pixelinvaders.ch
//
//Knight Rider effects  Modified for AldiStrip


uint8_t pixelsPerBlock;
uint8_t blockSize;

uint16_t kr=0;



//krSize  = the size of the moving parts in pixel
//howMany = how many movers should be active?
//krMode  = 0:single pixel update, 1: block update
void setupKnightRider(byte _krSize, byte _howMany, byte _krMode) {
  krSize = _krSize;
  howMany = _howMany;
  krMode = _krMode;
  
  pixelsPerBlock = strip.numPixels() / 16;
  blockSize = strip.numPixels() / pixelsPerBlock;
}


void loopKnightRider() {
  if (krMode==0) {
    //single pixel mode
    //uint32_t clearCol = complementaryColor();  
    for (uint16_t i=0; i < strip.numPixels(); i++) {
      //strip.setPixelColor(i, strip.Color(254-red,255- green,255- blue));
      strip.setPixelColor(i, strip.Color(red,green,blue));
      //setTintPixelColor(i, clearCol);
    }  
    drawKnightRider();    
  } else {
    //block mode
    uint16_t ofs = kr*pixelsPerBlock;
    uint32_t cc = Wheel((phase+kr)%255);
    for (uint16_t i=0; i<pixelsPerBlock; i++) {
      //setTintPixelColor(ofs++, cc);
      strip.setPixelColor(ofs++, Wheel(((phase+kr)%255)));
    }

    if (phase % 8==0) checkSwapDirection(blockSize);

  }
  strip.show();
}


//draw nrOf knight rider lines and check for updates
void drawKnightRider() {
  uint16_t ofs=0;
  
  for (uint16_t n=0; n<howMany; n++) {
    for (uint16_t i=kr; i<kr+krSize && i<strip.numPixels(); i++) {
      //setTintPixelColor(i+ofs, WHITE_COLOR);
      strip.setPixelColor(i+ofs, strip.Color(red2, green2, blue2));
    }
    ofs+=strip.numPixels()/howMany;
  }

  checkSwapDirection(strip.numPixels()/howMany);  
}

//should the direction swapped?
void checkSwapDirection(uint16_t lengthOfStrip) {
  if (krDirection==0) {
    kr++;
  } else {
    kr--;
  }
  
  if (kr>lengthOfStrip-krSize) {
    krDirection = 1;
    kr = lengthOfStrip-krSize;
  }
  
  if (kr==0) {
   krDirection = 0;
   kr = 0; 
  }    
}

uint32_t complementaryColor() {
  /*uint32_t c = 254-red;
  c <<= 8;
  c |= 255-green;
  c <<= 8;
  c |= 255-blue; */
  return strip.Color(254-red,255-green,255-blue);
}

void setTintPixelColor(uint16_t i, uint32_t c) {
  uint16_t b = c & 0xff;
  c >>= 8;
  uint16_t g = c & 0xff;
  c >>= 8;
  uint16_t r = c & 0xff;

  if (red >= 254 && green == 255 && blue == 255) {
    //no tint effect, no calculations needed
  } 
  else {
    //apply tint effect and SWAP color according to real cabeling
    r = r*(red+1) >> 8; if (r==255) {r=254;};
    g = g*(green+1) >> 8;
    b = b*(blue+1) >> 8;
  }

#ifdef USE_AUDIO_INPUT  
  //audio volume fx
  if (isAudioVolumeEnabled) {
    r *= audioVol;
    g *= audioVol;
    b *= audioVol;
  }
#endif
  //strip.setPixelColor(i, r, g, b);  //strip.Color(red, green, blue)
  strip.setPixelColor(i, strip.Color((uint8_t)r,(uint8_t) g,(uint8_t) b));
}
