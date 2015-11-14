void gaussianFilter(uchar* data, int width, int height)  
{  

 int i, j, index, sum;  

    int templates[9] = { 1, 2, 1,  
                        2, 4, 2,  
                        1, 2, 1 };
sum = height * width * sizeof(uchar);  
 uchar *tmpdata = (uchar*)malloc(sum);  
 memcpy((char*)tmpdata,(char*)data, sum);  
for(i = 1;i < height - 1;i++)  


    {   

        for(j = 1;j < width - 1;j++)  

        {   

            index = sum = 0;   

            for(int m = i - 1;m < i + 2;m++)  

            {   

                for(int n = j - 1; n < j + 2;n++)  

                {   

                   sum += tmpdata[m * width + n] *   
                   templates[index++];  
                }  
            }   
            data[i * width + j] = sum / 16;  

        }  

    }   

    free(tmpdata);  
}  
