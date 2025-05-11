#pragma once

#include <Eigen/dense>

#include <algorithm>
#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>

constexpr double PI = 3.14159265358979323846;


constexpr float degToRad(float deg) {
	return deg * PI / 180.0f;
}




	class ArcBall {
	private:
		Eigen::Vector3f TARGET_VECTOR, POSITION_VECTOR, UP_VECTOR;
		Eigen::Matrix4f PERSPECTIVE_MAT, VIEW_MAT;

		float YAW, PITCH;
		float DIST_FROM_TARGET;

		float FIELD_OF_VIEW;
		float a_sensitivity = 0.3f;
		float a_pan_speed = 0.4f;
		float a_scroll_sensitivity = 0.09f;


		void ProcMouseOrbit(float dx, float dy) {
			YAW += dx * a_sensitivity;
			PITCH= PITCH - dy * a_sensitivity;
			if (PITCH < -89.0f) {
				PITCH = -89.0f;
			}
			else if (PITCH > 89.0f) {
				PITCH = 89.0f;
			}
			RecalcPos();
		}

		void ProcMousePan(float dx, float dy) {
			Eigen::Vector3f forward = (TARGET_VECTOR - POSITION_VECTOR).normalized();
			Eigen::Vector3f right = (forward.cross(UP_VECTOR)).normalized();
			Eigen::Vector3f up = (right.cross(forward)).normalized();

			float panScale = DIST_FROM_TARGET * 0.002f;
			Eigen::Vector3f panOffset = (-right * dx + up * dy) * panScale;

			// Move both the camera and the target
			POSITION_VECTOR += panOffset;
			TARGET_VECTOR += panOffset;
		}


		void ProcMouseScroll(float dy) {
			DIST_FROM_TARGET = std::max(0.1f, DIST_FROM_TARGET - dy);
			RecalcPos();
		}

		void RecalcPos() {
			float radYaw = degToRad(YAW);
			float radPitch = degToRad(PITCH);
			Eigen::Vector3f direction = {
				cos(radPitch) * cos(radYaw),
				sin(radPitch),
				cos(radPitch) * sin(radYaw)
			};
			POSITION_VECTOR = TARGET_VECTOR - direction * DIST_FROM_TARGET;
		}

		Eigen::Matrix4f mine_lookAt(const Eigen::Vector3f& eye, const Eigen::Vector3f& center, const Eigen::Vector3f& up) {
			Eigen::Vector3f f = (center - eye).normalized();
			Eigen::Vector3f s = f.cross(up).normalized();
			Eigen::Vector3f u = s.cross(f);
		
			Eigen::Matrix4f view = Eigen::Matrix4f::Identity();
			view.block<1, 3>(0, 0) = s.transpose();
			view.block<1, 3>(1, 0) = u.transpose();
			view.block<1, 3>(2, 0) = -f.transpose();
			view(0, 3) = -s.dot(eye);
			view(1, 3) = -u.dot(eye);
			view(2, 3) = f.dot(eye);
		
			return view;
		}
		Eigen::Matrix4f mine_perspective(float fovY, float aspect, float zNear, float zFar) {
			float tanHalfFovy = std::tan(fovY / 2.0f);
		
			Eigen::Matrix4f proj = Eigen::Matrix4f::Zero();
			proj(0, 0) = 1.0f / (aspect * tanHalfFovy);
			proj(1, 1) = 1.0f / (tanHalfFovy);
			proj(2, 2) = -(zFar + zNear) / (zFar - zNear);
			proj(2, 3) = -2.0f * zFar * zNear / (zFar - zNear);
			proj(3, 2) = -1.0f;
		
			return proj;
		}
		
		Eigen::Matrix4f mine_ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
			Eigen::Matrix4f mat = Eigen::Matrix4f::Identity();
			mat(0, 0) = 2.0f / (right - left);
			mat(1, 1) = 2.0f / (top - bottom);
			mat(2, 2) = -2.0f / (zFar - zNear);
			mat(0, 3) = -(right + left) / (right - left);
			mat(1, 3) = -(top + bottom) / (top - bottom);
			mat(2, 3) = -(zFar + zNear) / (zFar - zNear);
			return mat;
		}

	public:
		ArcBall(){}//default
		ArcBall(float fov, float near, float far, float aspect) :
			POSITION_VECTOR(Eigen::Vector3f::Zero()),
			TARGET_VECTOR(Eigen::Vector3f::Zero()),
			UP_VECTOR(Eigen::Vector3f(0.0, 1.0, 0.0)),
			DIST_FROM_TARGET(2.0f),
			FIELD_OF_VIEW(fov),
			YAW(-90.0f),
			PITCH(0.0f)
		{
			PERSPECTIVE_MAT = mine_perspective(FIELD_OF_VIEW, aspect, near, far);
			RecalcPos();
		}

		~ArcBall() = default;

		void Update(GLFWwindow* window) {
			static double lastX = 0.0, lastY = 0.0;
			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);

			float dx = static_cast<float>(mouseX - lastX);
			float dy = static_cast<float>(mouseY - lastY);
			lastX = mouseX;
			lastY = mouseY;

			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
				ProcMouseOrbit(dx, dy);

			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
				ProcMousePan(dx * a_pan_speed, dy * a_pan_speed);
		}

		void setScrollDelta(float scrollY) {
			ProcMouseScroll(scrollY * a_scroll_sensitivity);  // apply scroll directly
		}

		Eigen::Vector3f getCamPos() const {
			return POSITION_VECTOR;
		}

		float* renderView() {
			VIEW_MAT = mine_lookAt(POSITION_VECTOR, TARGET_VECTOR, UP_VECTOR);
			//Eigen mat are column major matrix ,transpose it if opengl espects row.major
			return VIEW_MAT.data();
		}

		float* getProjMatrix() {
			return PERSPECTIVE_MAT.data();
		}
	};
