using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TaskManager
{
    class Comparer : IComparer
    {
        public int Index { get; set; }
        public SortOrder Direction { get; set; }
        public enum ValueType
        {
            String,
            Integer,
            Memory
        };
        public ValueType Type { get; set; }
        public Comparer(int column = 0)
        {
            this.Index = column;
            Direction = SortOrder.None;
        }

        public int Compare(object x, object y)
        {
            ListViewItem lviX = x as ListViewItem;
            ListViewItem lviY = y as ListViewItem;

            if (lviX == null && lviY == null) return 0;
            if (lviX == null) return -1;
            if (lviY == null) return 1;


            int result = 0;

            int x_value;
            int y_value;
            switch (Type)
            {
                case ValueType.Integer:
                    {
                        Int32.TryParse(lviX.SubItems[Index].Text, out x_value);
                        Int32.TryParse(lviY.SubItems[Index].Text, out y_value);
                        result = x_value.CompareTo(y_value);
                    }
                    break;
                case ValueType.Memory:
                    {
                        double x_memory, y_memory;
                        Double.TryParse(lviX.SubItems[Index].Text.Split(' ')[0], out x_memory);
                        Double.TryParse(lviY.SubItems[Index].Text.Split(' ')[0], out y_memory);
                        result = x_memory.CompareTo(y_memory);
                    }
                    break;
                case ValueType.String:
                    {
                        result = String.Compare
                        (
                            ((ListViewItem)x).SubItems[Index].Text,
                            ((ListViewItem)y).SubItems[Index].Text
                        );
                    }
                    break;
            }
            return Direction == SortOrder.Ascending ? result : -result;
        
        }

    }
}
