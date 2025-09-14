#!/usr/bin/env python3

import time
import asyncio
from time import sleep
from threading import Thread
from multiprocessing import Process
 
def work_sleep():
    for i in range(10000): time.sleep(0)
        
def work_i():
    lst = [i*i for i in range(1000000)]
    
async def async_work():
    lst = [i*i for i in range(1000000)]
        
async def async_work_sleep():
    for i in range(10000): sleep(0)
 


def use_multiprocessing():
    s_time = time.time()

    for i in range(100):
        process = Process(target=work_sleep())

    e_time = time.time()
    print(f"Multitprocessing work_sleep() execution time: {e_time - s_time:.4f} s")

    s_time = time.time()
    for i in range(100):
        process = Process(target=work_i())

    e_time = time.time()
    print(f"Multitprocessing work_i() execution time: {e_time - s_time:.4f} s")

def use_threading():
    s_time = time.time()

    for i in range(100):
        thread = Thread(target=work_sleep())

    e_time = time.time()
    print(f"Threading work_sleep() execution time: {e_time - s_time:.4f} s")

    s_time = time.time()

    for i in range(100):
        thread = Thread(target=work_i())

    e_time = time.time()
    print(f"Threading work_i() execution time: {e_time - s_time:.4f} s")

async def use_asyncio():
    s_time = time.time()

    for i in range(100):
        await async_work_sleep()
    
    e_time = time.time()
    print(f"Asyncio async_work_sleep() execution time: {e_time - s_time:.4f} s")

    s_time = time.time()

    for i in range(100):
        await async_work()
    
    e_time = time.time()
    print(f"Asyncio async_work() execution time: {e_time - s_time:.4f} s")

    
use_multiprocessing()
use_threading()
asyncio.run(use_asyncio())

