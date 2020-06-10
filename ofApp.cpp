#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(255);
	ofEnableDepthTest();

	this->cap.open("D:\\MP4\\video5.mp4");
	this->cap_size = cv::Size(1280, 720);

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(cv::Size(this->image.getWidth(), this->image.getHeight()), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);
}

//--------------------------------------------------------------
void ofApp::update() {

	cv::Mat src;
	this->cap >> src;
	if (src.empty()) {

		this->cap.set(cv::CAP_PROP_POS_FRAMES, 0);
		return;
	}

	cv::cvtColor(src, src, cv::COLOR_BGR2RGB);
	cv::resize(src, this->frame, this->cap_size);
	
	this->image.update();

	this->mesh_list.clear();

	int x_span = 80;
	int y_span = 5;
	for (int x = this->cap_size.width * -0.5; x < this->cap_size.width * 0.5; x += x_span) {

		ofMesh mesh;
		for (int y = this->cap_size.height * -0.5; y < this->cap_size.height * 0.5; y += y_span) {

			int index = mesh.getNumVertices();

			float z = ofMap(ofNoise(x * 0.01, y * 0.0015 + ofGetFrameNum() * 0.02), 0, 1, -100, 100);
			float next_z = ofMap(ofNoise(x * 0.01, (y + y_span) * 0.0015 + ofGetFrameNum() * 0.02), 0, 1, -100, 100);

			mesh.addVertex(glm::vec3(x, y, z));
			mesh.addVertex(glm::vec3(x + x_span, y, z));
			mesh.addVertex(glm::vec3(x + x_span, y + y_span, next_z));
			mesh.addVertex(glm::vec3(x, y + y_span, next_z));

			mesh.addTexCoord(glm::vec3(x + this->cap_size.width  * 0.5, y + this->cap_size.height * 0.5, 0));
			mesh.addTexCoord(glm::vec3(x + x_span + this->cap_size.width  * 0.5, y + this->cap_size.height * 0.5, 0));
			mesh.addTexCoord(glm::vec3(x + x_span + this->cap_size.width  * 0.5, y + y_span + this->cap_size.height * 0.5, 0));
			mesh.addTexCoord(glm::vec3(x + this->cap_size.width  * 0.5, y + y_span + this->cap_size.height * 0.5, 0));

			mesh.addIndex(index + 0); mesh.addIndex(index + 1); mesh.addIndex(index + 2);
			mesh.addIndex(index + 0); mesh.addIndex(index + 2); mesh.addIndex(index + 3);
		}

		this->mesh_list.push_back(mesh);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);

	for (auto& mesh : this->mesh_list) {

		this->image.bind();
		mesh.draw();
		this->image.unbind();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
