#pragma once
#include "ofMain.h"

struct BoneNode;
using BoneNodeRef = shared_ptr<BoneNode>;

struct BoneNode {
  ofMatrix4x4 local;
  ofMatrix4x4 diff;
  int rank = 0;
  BoneNode* parent;
  vector<BoneNodeRef> children;
  static BoneNode createRoot(ofPoint p);
  
  ofMatrix4x4 getMat() {
    if(!parent) return local;
    return local * parent->getMat();
  }
  ofMatrix4x4 getTransformed() {
    auto ld = local * diff;
    if(!parent) return ld;
    return ld * parent->getTransformed();
  }
  ofPoint getLocalToGlobalPos(ofPoint p) {
    return p * getTransformed();
  }
  ofPoint getPos() {
    return getLocalToGlobalPos(ofPoint(0));
  }
  BoneNodeRef append(const ofPoint& p) {
    auto child = make_shared<BoneNode>();
    child->rank = rank+1;
    child->parent = this;
    
    auto parentPos = getPos();
    ofMatrix4x4 rot;
    ofPoint z(0,0,1);
    rot.makeLookAtMatrix(ofPoint(0), -z, (p-parentPos).getRotated(90, z));
    ofMatrix4x4 mat;
    mat.setTranslation(p);
    
    child->local = rot * mat * getMat().getInverse();
    
    children.push_back(child);
    return child;
  }
  using Fn = function<void(BoneNode&)>;
  void baseUpdate(Fn fn) {
    _update(fn);
    for(auto& c : children) {
      c->baseUpdate(fn);
    }
  }
  void _update(Fn fn) {
    fn(*this);
  }
  void update(Fn fn) {
    if(!parent) baseUpdate(fn);
  }
};
