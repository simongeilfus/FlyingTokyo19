#pragma once
#include "cinder/app/App.h"

class ofApp : public ci::app::App {
  public:
	void setup() override;
	void update() override;
	void draw() override;
	void mouseDown( ci::app::MouseEvent event ) override;
};