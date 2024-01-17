import base64
import hashlib
import sys
from pathlib import Path
from typing import List

import schemas
from config import settings
from fastapi import UploadFile
from loguru import logger

import os
import math


class Storage:
    def __init__(self, is_test: bool):
        self.block_path: List[Path] = [
            Path(settings.UPLOAD_PATH) / f"{settings.FOLDER_PREFIX}-{i}"
            if is_test
            else Path(settings.UPLOAD_PATH) / f"{settings.FOLDER_PREFIX}-{i}"
            for i in range(settings.NUM_DISKS)
        ]
        self.__create_block()

    def __create_block(self):
        for path in self.block_path:
            logger.warning(f"Creating folder: {path}")
            path.mkdir(parents=True, exist_ok=True)

    async def file_integrity(self, filename: str) -> bool:
        """TODO: check if file integrity is valid
        file integrated must satisfy following conditions:
            1. all data blocks must exist
            2. size of all data blocks must be equal
            3. parity block must exist
            4. parity verify must success

        if one of the above conditions is not satisfied
        the file does not exist
        and the file is considered to be damaged
        so we need to delete the file
        """

        exist = 0
        block = []
        for i in range(settings.NUM_DISKS):
            check = os.path.join(settings.UPLOAD_PATH, f"{settings.FOLDER_PREFIX}-{i}")
            check_file = os.path.join(check, filename)

            if os.path.isfile(check_file):
                exist += 1
                file = open(check_file, "rb")
                block.append(bytearray(file.readline()))
        
        if exist < settings.NUM_DISKS:
            return False
        """
        first_length = len(block[0])  # Get the length of the first byte string
        all_same_length = all(len(bs) == first_length for bs in block[1:])

        if not all_same_length:
            return False
                
        parity = b''

        for i in range(settings.NUM_DISKS - 1):
            for j in len(block[i]):
                parity ^= block[i][j]

        parity_origin = block[settings.NUM_DISKS - 1]
        if parity != parity_origin:
            return False
        """
        return True

    async def create_file(self, file: UploadFile) -> schemas.File:
        # TODO: create file with data block and parity block and return it's schema

        filename = file.filename
        content_type = file.content_type
        content = await file.read()
        size = len(content)
        n = settings.NUM_DISKS

        if size > settings.MAX_SIZE:
            return schemas.File(
                name="too_big",
                size=size,
                checksum="too_big",
                content="too_big",
                content_type=content_type,
            )
        """
        check = os.path.join(settings.UPLOAD_PATH, f"{settings.FOLDER_PREFIX}-{0}")
        check_file = os.path.join(check, filename)

        if not os.path.isfile(check_file):
            return schemas.File(
                name="file_exist",
                size=size,
                checksum="file_exist",
                content="file_exist",
                content_type=content_type,
            )
        """
        
        whole_array = bytearray(content)

        file_size = int(len(whole_array) / (n - 1))
        mod = len(whole_array) % (n - 1)
        if mod > 0:
            file_size += 1
        
        
        parity = [bytearray([0] * file_size) for k in range(n)]

        arr_count = 0
        i = 0
        directories = os.listdir(settings.UPLOAD_PATH)
        for d in directories:
            d_path = os.path.join(settings.UPLOAD_PATH, d)
            file_path = os.path.join(d_path, filename)

            if i < (n - 1):
                for j in range(file_size):
                    if j == (file_size - 1):
                        if i < mod:
                            parity[i][j] = whole_array[arr_count]
                            arr_count += 1
                    else:
                        parity[i][j] = whole_array[arr_count]
                        arr_count += 1
                    parity[-1][j] ^= parity[i][j]
            # if i < n:
            with open(file_path, "wb") as file:
            # Write content to the file
                file.write(parity[i])

            i += 1

        
        """
        slice_point = [0]
        data_blocks = []

        first = size % (n - 1)
        for i in range(n):
            if i < first:
                file_size += math.floor(size / (n - 1)) + 1
                slice_point.append(file_size)
            else:
                file_size += math.floor(size / (n - 1))
                slice_point.append(file_size)

            if i < (n - 1):
                data_blocks.append(whole_array[slice_point[i]:slice_point[i + 1]:])
            else:
                data_blocks.append(whole_array[slice_point[i]::])
        
        i = 0
        max_data_size = max(len(block) for block in data_blocks)
        parity = b''
        directories = os.listdir(settings.UPLOAD_PATH)
        for d in directories:
            d_path = os.path.join(settings.UPLOAD_PATH, d)

            if os.path.isdir(d_path):
                file_path = os.path.join(d_path, filename)

                if i < n:
                    if len(data_blocks[i]) < max_data_size:
                        for z in range(len(data_blocks[i]), max_data_size):
                            # print(data_blocks[i])
                            data_blocks[i].append(0x00)

                    # data = ' '.join([str(elem) for elem in data_blocks[i]]) # change to a char
                    with open(file_path, "w") as file:
                    # Write content to the file
                        file.write(data_blocks[i])
                    
                    parity = bytes(x ^ y for x, y in zip(parity, data_blocks[i]))
            i += 1
        
        parity_data = ' '.join([str(parity)]) # change to a char
        with open(parity_file, "w") as file:
            file.write(parity_data)
        """
        
        # content = "お前はもう死んでいる!!!"ss
        return schemas.File(
            name=filename,
            size=size,
            checksum=hashlib.md5(content).hexdigest(),
            content=base64.b64encode(content),
            content_type=content_type,
        )

    async def retrieve_file(self, filename: str) -> bytes:
        # TODO: retrieve the binary data of file

        return b"".join("m3ow".encode() for _ in range(100))

    async def update_file(self, file: UploadFile) -> schemas.File:
        # TODO: update file's data block and parity block and return it's schema

        content = "何?!"
        return schemas.File(
            name="m3ow.txt",
            size=123,
            checksum=hashlib.md5(content.encode()).hexdigest(),
            content=base64.b64decode(content.encode()),
            content_type="text/plain",
        )

    async def delete_file(self, filename: str) -> None:
        # TODO: delete file's data block and parity block
        pass

    async def fix_block(self, block_id: int) -> None:
        # TODO: fix the broke block by using rest of block
        pass


storage: Storage = Storage(is_test="pytest" in sys.modules)
