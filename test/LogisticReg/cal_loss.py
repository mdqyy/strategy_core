# -*- coding: utf-8 -*-
from __future__ import with_statement
import sys
import math

class Loss(object):
  def __init__(self, weight_file, sample_file):
    self.dict_weight = {}
    self.weight_file = weight_file
    self.sample_file = sample_file
    self.loss = 0
    self.sample_num = 0
    
  def load_weight(self):
    with open(self.weight_file, 'r') as ipf:
      for line in ipf:
        line = line.strip()
        seg = line.split(' ')
        self.dict_weight[int(seg[0])] = float(seg[1])
  
  def get_p(self, inner):
    e = math.exp(inner)
    p = e/(1 + e)
    return p
  
  def predict(self):
    with open(self.sample_file, 'r') as ipf:
      for line in ipf:
        line = line.strip()
        if line.startswith('@'):
          continue
        self.sample_num += 1
        seg = line.split(' ')
        y = int(seg[0])
        inner = 0
        for i in xrange(1, len(seg)):
          tok = seg[i].split(':')
          (pos, value) = (int(tok[0]), float(tok[1]))
          inner += self.dict_weight[pos] * value
        p = self.get_p(inner)
        self.loss += math.pow((p - y), 2)
        print "reality:%f, predict:%f" % (y, p)

  def run(self):
    self.load_weight()
    self.predict()

  def get_loss(self):
    return self.loss
    
  def get_loss_ratio(self):
    return self.loss/self.sample_num

  def __def__(self):
    del self.dict_weight


if __name__ == '__main__':
  loss = Loss(sys.argv[1], sys.argv[2])
  loss.run()
  print "Loss ratio:%f" % (loss.get_loss_ratio())
  print "----------------------------------------------------------------------"
  sys.exit(0)
