from abc import ABC, abstractmethod
from typing import TypeVar, Generic, Optional, Any

T = TypeVar('T')

# 异常类定义 - 对应C++的outOfRange和badSize
class OutOfRange(Exception):
    """对应C++的outOfRange异常"""
    def __str__(self):
        return "ERROR! OUT OF RANGE."

class BadSize(Exception):
    """对应C++的badSize异常"""
    def __str__(self):
        return "ERROR! BAD SIZE."

# 抽象基类 - 对应C++的List模板类
class List(ABC, Generic[T]):
    """线性表抽象基类，定义所有线性表必须实现的接口"""
    
    @abstractmethod
    def clear(self) -> None:
        """清空线性表"""
        pass
    
    @abstractmethod
    def empty(self) -> bool:
        """判断线性表是否为空"""
        pass
    
    @abstractmethod
    def size(self) -> int:
        """返回线性表长度"""
        pass
    
    @abstractmethod
    def insert(self, i: int, value: T) -> None:
        """在位置i插入元素value"""
        pass
    
    @abstractmethod
    def remove(self, i: int) -> None:
        """删除位置i的元素"""
        pass
    
    @abstractmethod
    def search(self, value: T) -> int:
        """查找元素value，返回位置索引，未找到返回-1"""
        pass
    
    @abstractmethod
    def visit(self, i: int) -> T:
        """访问位置i的元素"""
        pass
    
    @abstractmethod
    def traverse(self) -> None:
        """遍历输出所有元素"""
        pass
    
    @abstractmethod
    def inverse(self) -> None:
        """逆置线性表"""
        pass

# 顺序表实现 - 对应C++的seqList
class SeqList(List[T]):
    """顺序表实现，使用动态数组存储"""
    
    def __init__(self, init_size: int = 100):
        """构造函数，对应C++构造函数"""
        if init_size <= 0:
            raise BadSize()
        self._max_size = init_size
        self._data: list[Optional[T]] = [None] * self._max_size  # 对应C++的elemType* data
        self._cur_length = 0  # 对应C++的curLength
    
    def _resize(self) -> None:
        """动态扩容，对应C++的resize()私有方法"""
        old_data = self._data
        self._max_size *= 2
        self._data: list[Optional[T]] = [None] * self._max_size
        for i in range(self._cur_length):
            self._data[i] = old_data[i]
    
    def clear(self) -> None:
        """清空顺序表"""
        self._cur_length = 0
    
    def empty(self) -> bool:
        """判断是否为空"""
        return self._cur_length == 0
    
    def size(self) -> int:
        """返回当前长度"""
        return self._cur_length
    
    def traverse(self) -> None:
        """遍历输出，格式与C++完全一致"""
        if self.empty():
            print("is empty")
        else:
            for i in range(self._cur_length):
                print(self._data[i], end=" ")
            print()
    
    def search(self, value: T) -> int:
        """查找元素，返回第一个匹配位置"""
        for i in range(self._cur_length):
            if value == self._data[i]:
                return i
        return -1
    
    def insert(self, i: int, value: T) -> None:
        """在位置i插入元素value"""
        if i < 0 or i > self._cur_length:
            raise OutOfRange()
        if self._cur_length == self._max_size:
            self._resize()
        
        # 元素后移，对应C++的循环
        for j in range(self._cur_length, i, -1):
            self._data[j] = self._data[j-1]
        
        self._data[i] = value
        self._cur_length += 1
    
    def remove(self, i: int) -> None:
        """删除位置i的元素"""
        if i < 0 or i > self._cur_length - 1:
            raise OutOfRange()
        
        # 元素前移，对应C++的循环
        for j in range(i, self._cur_length - 1):
            self._data[j] = self._data[j+1]
        
        self._cur_length -= 1
    
    def visit(self, i: int) -> T:
        """访问位置i的元素"""
        if i < 0 or i >= self._cur_length:
            raise OutOfRange()
        result = self._data[i]
        if result is None:
            raise RuntimeError("Unexpected None value in valid position")
        return result
    
    def inverse(self) -> None:
        """逆置顺序表，使用交换方式"""
        for i in range(self._cur_length // 2):
            # 对应C++中的tmp交换
            self._data[i], self._data[self._cur_length-1-i] = \
                self._data[self._cur_length-1-i], self._data[i]

# 单链表节点 - 对应C++的Node结构
class _LinkNode(Generic[T]):
    """单链表节点，对应C++的Node结构"""
    def __init__(self, data: Optional[T] = None, next_node: Optional['_LinkNode[T]'] = None):
        self.data = data  # 对应C++的data成员
        self.next = next_node  # 对应C++的next指针

# 单链表实现 - 对应C++的linkList
class LinkList(List[T]):
    """单链表实现，带头节点和尾指针"""
    
    def __init__(self):
        """构造函数，创建头节点和尾指针"""
        self._head = _LinkNode[T]()  # 对应C++的head，头节点
        self._tail = self._head  # 对应C++的tail，尾指针
        self._cur_length = 0  # 对应C++的curLength
    
    def _get_position(self, i: int) -> Optional[_LinkNode[T]]:
        """获取位置i的前一个节点，对应C++的getPosition()"""
        if i < -1 or i > self._cur_length - 1:
            return None
        
        p = self._head
        count = 0
        while count <= i and p is not None:
            p = p.next
            count += 1
        return p
    
    def clear(self) -> None:
        """清空链表，对应C++的clear()"""
        p = self._head.next
        while p is not None:
            tmp = p
            p = p.next
            del tmp  # 显式删除节点
        
        self._head.next = None
        self._tail = self._head
        self._cur_length = 0
    
    def empty(self) -> bool:
        """判断是否为空"""
        return self._head.next is None
    
    def size(self) -> int:
        """返回链表长度"""
        return self._cur_length
    
    def traverse(self) -> None:
        """遍历输出链表"""
        p = self._head.next
        while p is not None:
            print(p.data, end=" ")
            p = p.next
        print()
    
    def search(self, value: T) -> int:
        """查找元素value"""
        p = self._head.next
        count = 0
        while p is not None and p.data != value:
            p = p.next
            count += 1
        return count if p is not None else -1
    
    def insert(self, i: int, value: T) -> None:
        """在位置i插入元素value"""
        if i < 0 or i > self._cur_length:
            raise OutOfRange()
        
        # 找到插入位置的前一个节点
        if i == 0:
            p = self._head
        else:
            p = self._get_position(i-1)
            if p is None:
                raise OutOfRange()
        
        # 创建新节点并插入
        q = _LinkNode(value, p.next)
        p.next = q
        
        # 更新尾指针
        if p == self._tail:
            self._tail = q
        
        self._cur_length += 1
    
    def remove(self, i: int) -> None:
        """删除位置i的元素"""
        if i < 0 or i > self._cur_length - 1:
            raise OutOfRange()
        
        # 找到删除位置的前一个节点
        if i == 0:
            pre = self._head
        else:
            pre = self._get_position(i-1)
            if pre is None or pre.next is None:
                raise OutOfRange()
        
        p = pre.next
        if p is None:
            raise OutOfRange()
        
        # 更新尾指针
        if p == self._tail:
            self._tail = pre
            pre.next = None
        else:
            pre.next = p.next
        
        del p
        self._cur_length -= 1
    
    def visit(self, i: int) -> T:
        """访问位置i的元素"""
        if i < 0 or i >= self._cur_length:
            raise OutOfRange()
        
        p = self._head.next
        for count in range(i):
            if p is None:
                raise OutOfRange()
            p = p.next
            if p is None:
                raise OutOfRange()
        if p is None or p.data is None:
            raise OutOfRange()
        return p.data
    
    def inverse(self) -> None:
        """逆置链表，对应C++的算法"""
        p = self._head.next
        self._head.next = None
        
        if p is not None:
            self._tail = p  # 第一个节点成为尾节点
        
        # 头插法重建链表
        while p is not None:
            tmp = p.next
            p.next = self._head.next
            self._head.next = p
            p = tmp

# 双链表节点 - 对应C++的DoubleNode
class _DoubleLinkNode(Generic[T]):
    """双链表节点"""
    def __init__(self, data: Optional[T] = None, 
                 prior: Optional['_DoubleLinkNode[T]'] = None,
                 next_node: Optional['_DoubleLinkNode[T]'] = None):
        self.data = data  # 对应C++的data
        self.prior = prior  # 对应C++的prior指针
        self.next = next_node  # 对应C++的next指针

# 双链表实现 - 对应C++的doubleLinkList
class DoubleLinkList(List[T]):
    """双链表实现，使用头尾哨兵节点"""
    
    def __init__(self):
        """构造函数，创建头尾哨兵节点"""
        self._head = _DoubleLinkNode[T]()  # 头哨兵
        self._tail = _DoubleLinkNode[T]()  # 尾哨兵
        self._head.next = self._tail
        self._tail.prior = self._head
        self._cur_length = 0
    
    def _get_position(self, i: int) -> Optional[_DoubleLinkNode[T]]:
        """获取位置i的节点，对应C++的getPosition()"""
        if i < -1 or i > self._cur_length:
            return None
        
        p = self._head
        count = 0
        while count <= i and p is not None:
            p = p.next
            count += 1
        return p
    
    def clear(self) -> None:
        """清空双链表"""
        p = self._head.next
        self._head.next = self._tail
        self._tail.prior = self._head
        
        while p != self._tail and p is not None:
            tmp = p.next
            del p
            p = tmp
        
        self._cur_length = 0
    
    def empty(self) -> bool:
        """判断是否为空"""
        return self._head.next == self._tail
    
    def size(self) -> int:
        """返回链表长度"""
        return self._cur_length
    
    def traverse(self) -> None:
        """遍历输出双链表"""
        p = self._head.next
        while p != self._tail and p is not None:
            print(p.data, end=" ")
            p = p.next
        print()
    
    def search(self, value: T) -> int:
        """查找元素value"""
        p = self._head.next
        count = 0
        while p != self._tail and p is not None and p.data != value:
            p = p.next
            count += 1
        return count if p != self._tail else -1
    
    def insert(self, i: int, value: T) -> None:
        """在位置i插入元素value"""
        if i < 0 or i > self._cur_length:
            raise OutOfRange()
        
        p = self._get_position(i)
        if p is None or p.prior is None:
            raise OutOfRange()
        
        # 创建新节点并插入
        tmp = _DoubleLinkNode(value, p.prior, p)
        p.prior.next = tmp
        p.prior = tmp
        self._cur_length += 1
    
    def remove(self, i: int) -> None:
        """删除位置i的元素"""
        if i < 0 or i > self._cur_length - 1:
            raise OutOfRange()
        
        p = self._get_position(i)
        if p is None or p == self._tail:
            raise OutOfRange()
        
        # 删除节点
        if p.prior is not None and p.next is not None:
            p.prior.next = p.next
            p.next.prior = p.prior
        del p
        self._cur_length -= 1
    
    def visit(self, i: int) -> T:
        """访问位置i的元素"""
        if i < 0 or i >= self._cur_length:
            raise OutOfRange()
        
        p = self._head.next
        for count in range(i):
            if p is None:
                raise OutOfRange()
            p = p.next
        
        if p is None or p.data is None:
            raise OutOfRange()
        return p.data
    
    def inverse(self) -> None:
        """逆置双链表，对应C++算法"""
        p = self._head.next
        self._head.next = self._tail
        self._tail.prior = self._head
        
        # 头插法重建链表
        while p != self._tail and p is not None:
            tmp = p.next
            p.next = self._head.next
            p.prior = self._head
            self._head.next.prior = p
            self._head.next = p
            p = tmp

# 测试示例 - 与C++版本完全一致
if __name__ == "__main__":
    try:
        # 顺序表测试
        sl = SeqList[int](5)
        for i in range(5):
            sl.insert(i, i*2)
        print("顺序表初始: ", end="")
        sl.traverse()
        sl.inverse()
        print("顺序表逆置: ", end="")
        sl.traverse()

        # 单链表测试
        ll = LinkList[int]()
        for i in range(5):
            ll.insert(i, i+1)
        print("单链表初始: ", end="")
        ll.traverse()
        ll.remove(2)
        print("删除第三个元素: ", end="")
        ll.traverse()

        # 双链表测试
        dl = DoubleLinkList[int]()
        for i in range(3):
            dl.insert(i, i*3)
        print("双链表初始: ", end="")
        dl.traverse()
        dl.inverse()
        print("双链表逆置: ", end="")
        dl.traverse()
        
    except (OutOfRange, BadSize) as e:
        print(f"异常: {e}")