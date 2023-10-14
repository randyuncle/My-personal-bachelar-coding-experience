# -*- coding: utf-8 -*-
"""
Created on Tue Jan 25 20:31:10 2023

@author: Jeremy Chang
"""

import os
import logging
import argparse

import torch
from torch.utils.data import Dataset, DataLoader
from torch.optim import AdamW
from transformers import AutoConfig, AutoTokenizer, AutoModelForSequenceClassification
from transformers import get_cosine_schedule_with_warmup, WEIGHTS_NAME, CONFIG_NAME
from transformers import logging as tf_logging

from sklearn.metrics import accuracy_score
import matplotlib.pyplot as plt


logging.basicConfig(format = '%(asctime)s - %(levelname)s - %(name)s -   %(message)s',
                    datefmt = '%m/%d/%Y %H:%M:%S',
                    level = logging.INFO)
logger = logging.getLogger(__name__)

class Dataset(Dataset):
    def __init__(self, text_file, label_file):
        super().__init__()
        self.text = open(text_file, 'r', encoding="utf-8").readlines()
        self.label = open(label_file, 'r', encoding="utf-8").readlines()
        self.label_map = {"very negative": 0, "negative": 1, "neutral": 2, "positive": 3, "very positive": 4}
        
    def __len__(self):
        return len(self.text)
    
    def __getitem__(self, index):
        text = str(self.text[index].strip())
        label = self.label_map[self.label[index].strip()]
        
        return {'text': text, 'label': label}

class bertClassificationCollator(object):
    def __init__(self, tokenizer, max_seq_len=None):
        self.tokenizer = tokenizer
        self.max_seq_len = max_seq_len

        return
    
    def __call__(self, sequences):
        texts = [sequence['text'] for sequence in sequences]    
        labels = [sequence['label'] for sequence in sequences]
        inputs = self.tokenizer(text=texts,
                                return_tensors='pt',
                                padding=True,
                                truncation=True,
                                max_length=self.max_seq_len)

        inputs.update({'labels': torch.tensor(labels)})

        return inputs


def train(model, dataloader, optimizer, scheduler, device_):
    model.train()
    
    prediction_labels = []
    true_labels = []
    
    total_loss = []
    
    for batch in dataloader:
        true_labels += batch['labels'].numpy().flatten().tolist()
        batch = {k:v.to(device_) for k, v in batch.items()}
        
        outputs = model(**batch)
        loss, logits = outputs[:2]
        preds = torch.argmax(logits,1)
        preds = preds.detach().cpu().numpy()
        total_loss.append(loss.item())
        
        optimizer.zero_grad()
        loss.backward()
        torch.nn.utils.clip_grad_norm_(model.parameters(), 1.0) # prevent exploding gradient

        optimizer.step()
        scheduler.step()
        
        prediction_labels += preds.flatten().tolist()
    
    
    return true_labels, prediction_labels, total_loss

def validation(model, dataloader, device_):
    model.eval()
    
    prediction_labels = []
    true_labels = []
    
    total_loss = []
    
    for batch in dataloader:
        true_labels += batch['labels'].numpy().flatten().tolist()
        batch = {k:v.type(torch.long).to(device_) for k, v in batch.items()}
        
        with torch.no_grad():
            outputs = model(**batch)
            loss, logits = outputs[:2]
            preds = torch.argmax(logits,1)
            preds = preds.detach().cpu().numpy()
            total_loss.append(loss.item())

            prediction_labels += preds.flatten().tolist()
        
    return true_labels, prediction_labels, total_loss

def test(model, dataloader, device_):
    model.eval()
    
    prediction_labels = []
    true_labels = []
    
    total_loss = []
    
    for batch in dataloader:
        true_labels += batch['labels'].numpy().flatten().tolist()
        batch = {k:v.type(torch.long).to(device_) for k, v in batch.items()}
        
        with torch.no_grad():
            outputs = model(**batch)
            loss, logits = outputs[:2]
            preds = torch.argmax(logits,1)
            preds = preds.detach().cpu().numpy()
            total_loss.append(loss.item())

            prediction_labels += preds.flatten().tolist()
        
    return true_labels, prediction_labels, total_loss

def main():
    parser = argparse.ArgumentParser()

    ## Required parameters
    parser.add_argument("--train_file",
                        default=None,
                        type=str,
                        required=False)
    parser.add_argument("--train_label",
                        default=None,
                        type=str,
                        required=False)
    parser.add_argument("--valid_file",
                        default=None,
                        type=str,
                        required=False)
    parser.add_argument("--valid_label",
                        default=None,
                        type=str,
                        required=False)
    parser.add_argument("--test_file",
                        default=None,
                        type=str,
                        required=False)
    parser.add_argument("--test_label",
                        default=None,
                        type=str,
                        required=False)
    parser.add_argument("--output_dir",
                        default=None,
                        type=str,
                        required=True,
                        help="The output directory where the model predictions and checkpoints will be written.")
    parser.add_argument("--num_train_epochs",
                    default=10,
                    type=int,
                    help="Total number of training epochs to perform.")
    parser.add_argument("--batch_size",
                    default=32,
                    type=int,
                    help="Total batch size for training.")
    parser.add_argument("--no_cuda",
                        action='store_true',
                        help="Whether not to use CUDA when available")
    parser.add_argument("--local_rank",
                        type=int,
                        default=-1,
                        help="local_rank for distributed training on gpus")
    parser.add_argument('--seed',
                    type=int,
                    default=42,
                    help="random seed for initialization")
    parser.add_argument("--train",
                        action='store_true',
                        help="Whether not to use train when available")
    parser.add_argument("--test",
                        action='store_true',
                        help="Whether not to use test when available")
    parser.add_argument("--load_model",
                        default=None,
                        type=str,
                        required=False)
    
    
    args = parser.parse_args()
    
    if args.local_rank == -1 or args.no_cuda:
        device = torch.device("cuda" if torch.cuda.is_available() and not args.no_cuda else "cpu")
        n_gpu = torch.cuda.device_count()
    else:
        torch.cuda.set_device(args.local_rank)
        device = torch.device("cuda", args.lfocal_rank)
        n_gpu = 1
        # Initializes the distributed backend which will take care of sychronizing nodes/GPUs
        torch.distributed.init_process_group(backend='nccl')
        
    if n_gpu > 0:
        torch.cuda.manual_seed_all(args.seed)

    # construct dirs
    if not os.path.exists(args.output_dir):
        os.makedirs(args.output_dir)
    
    if not os.path.exists(args.output_dir+'/bert_config'):
        os.makedirs(args.output_dir+'/bert_config')
    
    if not os.path.exists(args.output_dir+'/bert_model'):
        os.makedirs(args.output_dir+'/bert_model')

    if not os.path.exists(args.output_dir+'/bert_result'):
        os.makedirs(args.output_dir+'/bert_result')

    if not os.path.exists(args.output_dir+'/bert_png'):
        os.makedirs(args.output_dir+'/bert_png')

    tf_logging.set_verbosity_error()    
    
    model_config = AutoConfig.from_pretrained('prajjwal1/bert-tiny', num_labels=5) # Regression Classification
    model = AutoModelForSequenceClassification.from_pretrained('prajjwal1/bert-tiny', config=model_config)
    tokenizer = AutoTokenizer.from_pretrained('prajjwal1/bert-tiny')
    device = 'cuda' if torch.cuda.is_available() else 'cpu'
    model.to(device)
    
    bertclassificationcollator = bertClassificationCollator(tokenizer=tokenizer,
                                                                  max_seq_len=64)
    
    outfile_name = str(args.batch_size)+str(int(args.num_train_epochs))
    output_model_file = os.path.join(args.output_dir+'/bert_model', outfile_name+WEIGHTS_NAME)
        
    if args.load_model:
        logger.info("***** Model Loading *****")
        logger.info(str(args.load_model))
        model.load_state_dict(torch.load(args.load_model))
        model.eval()
        args.train = False
    
    if args.train: 
        train_dataset = Dataset(args.train_file, args.train_label)
        val_dataset = Dataset(args.valid_file, args.valid_label)     
        
        logger.info("***** Examples *****")
        for i in range(5):       
            print(train_dataset.__getitem__(i))
        print('--------------')
    
        train_size = int(len(train_dataset))
        val_size = int(len(val_dataset))
        
        logger.info("***** Data Loading *****")
        logger.info("  Batch size = %d", args.batch_size)
        logger.info("  Num training examples = %d", train_size)
        logger.info("  Num validation examples = %d", val_size)
            
        train_dataloader = DataLoader(dataset=train_dataset,
                                      batch_size=args.batch_size,
                                      shuffle=True,
                                      collate_fn=bertclassificationcollator)
        val_dataloader = DataLoader(dataset=val_dataset,
                                    batch_size=args.batch_size,
                                    shuffle=False,
                                    collate_fn=bertclassificationcollator)
    
        total_epochs = args.num_train_epochs
        
        param_optimizer = list(model.named_parameters())
        no_decay = ['bias', 'LayerNorm.bias', 'LayerNorm.weight']
        optimizer_grouped_parameters = [
            {'params': [p for n, p in param_optimizer if not any(nd in n for nd in no_decay)], 'weight_decay': 0.01},
            {'params': [p for n, p in param_optimizer if any(nd in n for nd in no_decay)], 'weight_decay': 0.0}
        ]
        optimizer = AdamW(optimizer_grouped_parameters,
                          lr=1e-5,
                          eps=1e-8)
        
        num_train_steps = len(train_dataloader) * total_epochs
        num_warmup_steps = int(num_train_steps * 0.1) 
        lr_scheduler = get_cosine_schedule_with_warmup(optimizer,num_warmup_steps=num_warmup_steps,num_training_steps = num_train_steps)
        
        all_loss = {'train_loss': [], 'val_loss': []}
        all_acc = {'train_acc': [], 'val_acc': []}
        best_acc = 0
        
        for epoch in range(total_epochs):
            
            y, y_pred, train_loss = train(model, train_dataloader, optimizer, lr_scheduler, device)
            train_acc = accuracy_score(y, y_pred)
            
            y, y_pred, val_loss = validation(model, val_dataloader, device)
            val_acc = accuracy_score(y, y_pred)
            
            all_loss['train_loss'] += train_loss
            all_loss['val_loss'] += val_loss
            
            all_acc['train_acc'].append(train_acc)
            all_acc['val_acc'].append(val_acc)
            
            logger.info(f'Epoch: {epoch}, train_loss: {torch.tensor(train_loss).mean():.3f}, train_acc: {train_acc:.3f}, val_loss: {torch.tensor(val_loss).mean():.3f}, val_acc: {val_acc:.3f}')
            
            if best_acc < val_acc:
                logger.info("***** Saving best model *****")
                best_acc = val_acc
                # Save a trained model and the associated configuration
                model_to_save = model.module if hasattr(model, 'module') else model  # Only save the model it-self
                torch.save(model_to_save.state_dict(), output_model_file)
                output_config_file = os.path.join(args.output_dir+'/bert_config', outfile_name+CONFIG_NAME)
                result_out_file = os.path.join(args.output_dir+'/bert_result', outfile_name+".txt")
    
                with open(output_config_file, 'w') as f:
                    f.write(model_to_save.config.to_json_string())
                with open(result_out_file, 'w') as f:
                    f.write(f"train_acc: {train_acc:.3f}\nval_acc: {val_acc:.3f}\n")
                    
        fig = plt.figure(figsize=(20,20))
        a = fig.add_subplot(4, 1, 1)
        b = fig.add_subplot(4, 1, 2)
        c = fig.add_subplot(2, 1, 2)
        a.plot(all_loss['train_loss'])
        b.plot(all_loss['val_loss'])
        c.plot(all_acc['train_acc'])
        c.plot(all_acc['val_acc'])
        c.set(xlabel='epoch', ylabel='acc')
        c.legend(['train', 'val'])
        fig.savefig(args.output_dir+'/bert_png/'+outfile_name+'.png')
    
    if args.test:
        logger.info("***** Testing *****")
        test_dataset = Dataset(args.test_file, args.test_label)
        test_size = int(len(test_dataset))
        logger.info("  Num testing examples = %d\n", test_size)
        test_dataloader = DataLoader(dataset=test_dataset,
                                batch_size=args.batch_size,
                                shuffle=False,
                                collate_fn=bertclassificationcollator)
        
        if args.load_model == False:
            model = model.load_state_dict(torch.load(output_model_file))
            model.eval()
        y, y_pred, _ = test(model, test_dataloader, device)
        test_acc = accuracy_score(y, y_pred)
        logger.info(f'test_acc: {test_acc:.3f}')
        
        result_out_file = os.path.join(args.output_dir+'/bert_result/test.txt')

        with open(result_out_file, 'w') as f:
            f.write(f"test_acc: {test_acc:.3f}\n")
    
if __name__ == "__main__":
    print("cuda", torch.cuda.is_available())
    main()
