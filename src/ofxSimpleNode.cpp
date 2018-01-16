#include "ofxSimpleNode.h"
BoneNode BoneNode::createRoot(ofPoint p) {
  BoneNode n;
  n.rank = 0;
  n.local = n.diff = ofMatrix4x4();
  return move(n);
}
