using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApp1
{
    public class MatrixMulti
    {
        double[,] m1 = null;
        double[,] m2 = null;
        double[,] resultM = null;
        String resultPath = "";

        public MatrixMulti(String m1Path, String m2Path, String resultPath)
        {
            m1 = ReadMatrixFromFile(m1Path);
            m2 = ReadMatrixFromFile(m2Path);
            this.resultPath = resultPath;
            resultM = Multiplication();
            SaveResultToFile();
        }

        double[,] ReadMatrixFromFile(String mPath)
        {
            String input = File.ReadAllText(mPath);
            String[] lines = input.Split('\n');
            String firstLine = lines[0];
            String[] size = firstLine.Split(' ');
            int i = int.Parse(size[0]);
            int j = int.Parse(size[1]);
            double[,] result = new double[i, j];
            lines = lines.Skip(1).ToArray();
            i = 0; j = 0;
            foreach (var row in lines)
            {
                j = 0;
                foreach (var col in row.Trim().Split(' '))
                {
                    var num = col.Trim();
                    if (num != "")
                    {
                        result[i, j] = double.Parse(num);
                        j++;
                    }
                }
                i++;
            }

            return result;
        }

        double[,] Multiplication()
        {
            int aRows = m1.GetLength(0); int aCols = m1.GetLength(1);
            int bRows = m2.GetLength(0); int bCols = m2.GetLength(1);
            if (aCols != bRows)
                throw new Exception("Matrix size error");

            double[,] result = new double[aRows, bCols];

            Parallel.For(0, aRows, i =>
            {
                //for (int i = 0; i < aRows; i++)
                for (int j = 0; j < bCols; ++j)
                {
                    for (int k = 0; k < aCols; ++k)
                    {
                        result[i, j] += m1[i, k] * m2[k, j];
                    }
                }
            });

            return result;
        }

        void SaveResultToFile()
        {
            System.IO.StreamWriter streamWriter = new System.IO.StreamWriter(resultPath);
            string output = "";
            for (int i = 0; i < resultM.GetLength(0); i++)
            {
                for (int j = 0; j < resultM.GetLength(1); j++)
                {
                    output += resultM[i, j].ToString() + " ";
                }
                streamWriter.WriteLine(output);
                output = "";
            }
            streamWriter.Close();
        }
    }
}
