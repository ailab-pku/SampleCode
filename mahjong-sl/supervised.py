# Yunlong Lu created at 2021/11/28, advised by Wenxin Li, PKU AI Lab

from dataset import MahjongGBDataset
from torch.utils.data import DataLoader
from model import CNNModel
import torch.nn.functional as F
import torch
import os

if __name__ == '__main__':
    logdir = '/model/'
    os.mkdir(logdir + 'checkpoint')
    
    # Load dataset
    splitRatio = 0.9
    batchSize = 1024
    trainDataset = MahjongGBDataset(0, splitRatio, True)
    validateDataset = MahjongGBDataset(splitRatio, 1, False)
    loader = DataLoader(dataset = trainDataset, batch_size = batchSize, shuffle = True)
    vloader = DataLoader(dataset = validateDataset, batch_size = batchSize, shuffle = False)
    
    # Load model
    model = CNNModel().to('cuda')
    optimizer = torch.optim.Adam(model.parameters(), lr = 5e-4)
    
    # Train and validate
    for e in range(16):
        print('Epoch', e)
        model.train(True)
        torch.save(model.state_dict(), logdir + 'checkpoint/%d.pkl' % e)
        for i, d in enumerate(loader):
            input_dict = {'observation': d[0].cuda(), 'action_mask': d[1].cuda()}
            logits = model(input_dict)
            loss = F.cross_entropy(logits, d[2].long().cuda())
            if i % 128 == 0:
                print('Iteration %d/%d'%(i, len(trainDataset) // batchSize + 1), 'policy_loss', loss.item())
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
        print('Run validation:')
        model.train(False)
        correct = 0
        for i, d in enumerate(vloader):
            input_dict = {'observation': d[0].cuda(), 'action_mask': d[1].cuda()}
            with torch.no_grad():
                logits = model(input_dict)
                pred = logits.argmax(dim = 1)
                correct += torch.eq(pred, d[2].cuda()).sum().item()
        acc = correct / len(validateDataset)
        print('Epoch', e + 1, 'Validate acc:', acc)