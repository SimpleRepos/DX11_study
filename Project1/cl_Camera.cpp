#include "cl_Camera.h"

using namespace DirectX;

Camera::Camera() {
  setEyePos(0, 0, 0);
  setTargetPos(0, 0, 1);

  setFovY(XM_PIDIV4);
  setAspectRatio(4, 3);
  setDepthLimits(1, 1000);
}

XMFLOAT4X4 Camera::getView() const {
  if(dirtyView) {
    XMVECTOR eye = XMLoadFloat3(&eyePos);
    XMVECTOR target = XMLoadFloat3(&lookTarget);
    static const XMVECTOR up = XMVectorSet(0, 1, 0, 0);
    XMMATRIX v;
    switch(lookMode) {
    case     AT:
    {
      auto derp = DirectX::XMVectorEqual(eye, target);
      if(derp.m128_f32[0] && derp.m128_f32[1] && derp.m128_f32[2]) { break; }
      v = DirectX::XMMatrixLookAtLH(eye, target, up);
      break;
    }
    case TOWARD: v = DirectX::XMMatrixLookToLH(eye, target, up); break;
    }
    XMStoreFloat4x4(&view, v);

    dirtyView = false;
  }

  return view;
}

XMFLOAT4X4 Camera::getProj() const {
  if(dirtyProj) {
    XMMATRIX p = DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
    XMStoreFloat4x4(&proj, p);

    dirtyProj = false;
  }

  return proj;
}

XMFLOAT4X4 Camera::getViewProj() const {
  DirectX::XMMATRIX v = XMLoadFloat4x4(&getView());
  DirectX::XMMATRIX p = XMLoadFloat4x4(&getProj());

  XMFLOAT4X4 ret;
  XMStoreFloat4x4(&ret, v * p);

  return ret;
}

DirectX::XMFLOAT4X4 Camera::getTransposedWVP(const DirectX::XMFLOAT4X4& world) {
  return getTransposedWVP(XMLoadFloat4x4(&world));
}

DirectX::XMFLOAT4X4 Camera::getTransposedWVP(const DirectX::XMMATRIX& world) {
  DirectX::XMMATRIX vp = XMLoadFloat4x4(&getViewProj());
  DirectX::XMFLOAT4X4 ret;
  XMStoreFloat4x4(&ret, DirectX::XMMatrixTranspose(world * vp));
  return ret;
}

DirectX::XMFLOAT3 Camera::getRightwardDirection() {
  XMMATRIX ivp = XMMatrixInverse(nullptr, XMLoadFloat4x4(&getViewProj()));

  XMVECTOR right = XMVectorSet(1, 0, 0, 0);
  right = XMVector3Normalize(XMVector4Transform(right, ivp));

  XMFLOAT3 retval;
  XMStoreFloat3(&retval, right);

  return retval;
}

DirectX::XMFLOAT3 Camera::getForwardDirection() {
  if(lookMode == TOWARD) { return lookTarget; }

  XMVECTOR target = XMLoadFloat3(&lookTarget);
  XMVECTOR eye = XMLoadFloat3(&eyePos);
  XMVECTOR dir = target - eye;
  dir = XMVector3Normalize(dir);

  XMFLOAT3 retval;
  XMStoreFloat3(&retval, dir);

  return retval;
}

DirectX::XMFLOAT3 Camera::getFlatForward() {
  XMFLOAT3 fwd = getForwardDirection();
  XMVECTOR vec = XMVector3Normalize(XMVectorSet(fwd.x, 0, fwd.z, 0));
  XMStoreFloat3(&fwd, vec);
  return fwd;
}

DirectX::XMFLOAT3 Camera::getFlatBackward() {
  XMFLOAT3 f3 = getForwardDirection();
  XMVECTOR vec = XMVector3Normalize(XMVectorSet(-f3.x, 0, -f3.z, 0));
  XMStoreFloat3(&f3, vec);
  return f3;
}

void Camera::setEyePos(Arm arm) {
  //len = length of camera arm
  // ev = elevation angle of camera arm
  // or = orbital angle of camera arm
  //   (v   *        rotX)        *            rotY
  // [ 0 ]   [1,       0,       0]    [ cos(or), 0, sin(or)]
  //([ 0 ] * [0, cos(ev),-sin(ev)]) * [       0, 1,       0]
  // [len]   [0, sin(ev), cos(ev)]    [-sin(or), 0, cos(or)]
  // simplifies to vector:
  //[len *  cos(ev) * sin(or)]
  //[len * -sin(ev)]
  //[len *  cos(ev) * cos(or)]
  //adding this vector to the target's world position and putting the cam in
  //look-at mode gives a decent camera control system

  float lenCosPhi = cosf(arm.elevationRadians) * arm.length; //calc'ed once; used twice
  XMFLOAT3 camPos{
    (lenCosPhi * sinf(arm.orbitRadians)) + arm.targetPos.x,
    (arm.length * -sinf(arm.elevationRadians)) + arm.targetPos.y,
    (lenCosPhi * cosf(arm.orbitRadians)) + arm.targetPos.z
  };

  setEyePos(camPos);
  setTargetPos(arm.targetPos);
}

void Camera::setEyePos(XMFLOAT4 pos) {
  setEyePos(XMFLOAT3(pos.x, pos.y, pos.z));
}

void Camera::setEyePos(XMFLOAT3 pos) {
  dirtyView = true;
  eyePos = pos;
}

void Camera::setEyePos(float x, float y, float z) {
  setEyePos(XMFLOAT3{ x, y, z });
}

void Camera::setTargetPos(XMFLOAT3 pos) {
  dirtyView = true;
  lookTarget = pos;
  lookMode = AT;
}

void Camera::setTargetPos(float x, float y, float z) {
  setTargetPos({ x, y, z });
}

void Camera::setTargetDir(XMFLOAT3 pos) {
  dirtyView = true;
  lookTarget = pos;
  lookMode = TOWARD;
}

void Camera::setTargetDir(float x, float y, float z) {
  setTargetDir({ x, y, z });
}

void Camera::setFovY(float fov) {
  dirtyProj = true;
  fovY = fov;
}

void Camera::setAspectRatio(float ratio) {
  dirtyProj = true;
  aspectRatio = ratio;
}

void Camera::setAspectRatio(float width, float height) {
  dirtyProj = true;
  setAspectRatio(width / height);
}

void Camera::setDepthLimits(float zNear, float zFar) {
  dirtyProj = true;
  nearZ = zNear;
  farZ = zFar;
}



