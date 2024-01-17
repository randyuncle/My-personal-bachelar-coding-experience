import schemas
from fastapi import APIRouter, Response, UploadFile, status
from fastapi.exceptions import HTTPException
from storage import storage

router = APIRouter()


@router.post(
    "/",
    status_code=status.HTTP_201_CREATED,
    response_model=schemas.File,
    name="file:create_file",
)
async def create_file(file: UploadFile) -> schemas.File:
    msg = await storage.create_file(file)
    if msg.name == "too_big":
        raise HTTPException(
            status_code=413,
            detail="File too large",
        )
    elif msg.name == "file_exist":
        raise HTTPException(
            status_code=409,
            detail="File already exists",
        )
    
    return msg


@router.get("/", status_code=status.HTTP_200_OK, name="file:retrieve_file")
async def retrieve_file(filename: str) -> Response:
    # TODO: Add headers to ensure the filename is displayed correctly
    #       You should also ensure that enables the judge to download files directly
    return Response(
        await storage.retrieve_file(filename),
        media_type="application/octet-stream",
        headers={},
    )


@router.put("/", status_code=status.HTTP_200_OK, name="file:update_file")
async def update_file(file: UploadFile) -> schemas.File:
    return await storage.update_file(file)


@router.delete("/", status_code=status.HTTP_200_OK, name="file:delete_file")
async def delete_file(filename: str) -> str:
    await storage.delete_file(filename)
    return schemas.Msg(detail="File deleted")
