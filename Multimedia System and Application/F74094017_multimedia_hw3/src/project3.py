from matplotlib import pyplot as plt
import numpy as np
import cv2

img = cv2.imread('../picture/Histogram+Edge.bmp', cv2.IMREAD_GRAYSCALE)

"""
Histogram Equalization
"""
# reference: https://web.archive.org/web/20151219221513/http://www.janeriksolem.net/2009/06/histogram-equalization-with-python-and.html
def histogram_equalize(img): 
    # make the histogram of original image by numpy
    hist, bins = np.histogram(img.flatten(), 256, [0, 256])

    # calculate the cumulative sum
    cdf = hist.cumsum()
    # normalize the cdf
    cdf = cdf * 255 / cdf[-1]

    # then, equalize the image by the `cdf`` we get
    equalized_img = np.interp(img.flatten(), bins[:-1], cdf).reshape(img.shape)
    # make the histogram of equalized image by numpy
    equalized_hist, _ = np.histogram(equalized_img.flatten(), 256, [0, 256])

    return hist, equalized_hist, equalized_img

hist, equalized_hist, equalized_img = histogram_equalize(img)

cv2.imwrite('../figure/histogram_equalization.png', equalized_img)

plt.figure(figsize=(10, 7))
plt.plot(hist, color='black')
plt.title("Original Histogram")
plt.savefig('../figure/Original_Histogram.png')

plt.figure(figsize=(10, 7))
plt.plot(equalized_hist, color='black')
plt.title("Equalized Histogram")
plt.savefig('../figure/Equalized_Histogram.png')

"""
Sobel Operation
"""
def sobel_operation(img): 
    # Init. the gx and gy edge (both are 2D-array)
    gx = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]])
    gy = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])

    # Init. the df(x,y)/dx and df(x,y)/dy for sobel operator
    x, y = img.shape
    sx = np.zeros((x, y))
    sy = np.zeros((x, y))

    # calculate the integral
    # similar to the equation in P.59 of the ppt
    for r in range(x): 
        for c in range(y): 
            for s in range(3): 
                for t in range(3): 
                    if r + s < x and c + t < y: # prevent out of bound
                        sx[r][c] += gx[s][t] * img[r + s][c + t]
                        sy[r][c] += gy[s][t] * img[r + s][c + t]
    
    # G = |df(x,y)/dx| + |df(x,y)/dy|
    for r in range(x): 
        for c in range(y): 
            sx[r][c] = abs(sx[r][c])
            sy[r][c] = abs(sy[r][c])

    return sx + sy

sobel = sobel_operation(img)
cv2.imwrite('../figure/sobel_operator.png', sobel)