# C Container Collection

## Synopsis

A collection of **high-performance** containers implemented by C macros.

## Example

```C
cc_dllst(msg, char*);
cc_dllst_iter(msg_iter, msg);

cc_dllst_push_back(msg, "Hello everyone:\n");
cc_dllst_push_back(msg, "Welcome to C Container Collection Project!\n");

cc_dllst_trav(msg_iter, msg)
    printf("%s", ***msg_iter);

cc_dllst_dealloc(msg);
```

## Macros List

<table>
<tr>
	<th rowspan="2">Container</th>
	<th colspan="3">Operation</th>
	<th rowspan="2">Description</th>
</tr>
<tr>
	
	<th>Class</th><th>Return</th><th>Macro Function Prototype</th>
	
</tr>
<tr>
	<th rowspan="17">doubly</br>linked list</br>(dllst)</th>
	<td>constructor</td><td align="right">void</td><td>cc_dllst (Object, TYPE);</td>
	<td></td>
</tr>
<tr>
	
	<td>destructor</td><td align="right">void</td><td>cc_dllst_dealloc (Object);</td>
	<td></td>
</tr>
<tr>
	
	<td rowspan="9">iterators</td><td align="right">void</td><td>cc_dllst_iter (Iter, Object);</td>
	<td></td>
</tr>
<tr>
	
	<td align="right">bool</td><td>cc_dllst_iter_is_valid (Iter);</td>
	<td></td>
</tr>
<tr>
	
	<td align="right">TYPE&</td><td>cc_dllst_iter_dereference (Iter);</td>
	<td></td>
</tr>
<tr>
	
	<td align="right">void</td><td>cc_dllst_iter_copy (Iter_Dst, Iter_Src);</td>
	<td></td>
</tr>
<tr>
	
	<td align="right">void</td><td>cc_dllst_iter_clear (Iter);</td>
	<td></td>
</tr>
<tr>
	
	<td align="right">Iter</td><td>cc_dllst_iter_begin (Iter, Object);</td>
	<td></td>
</tr>
<tr>
	
	<td align="right">Iter</td><td>cc_dllst_iter_end (Iter, Object);</td>
	<td></td>
</tr>
<tr>
	
	<td align="right">Iter</td><td>cc_dllst_iter_incr (Iter);</td>
	<td></td>
</tr>
<tr>
	
	<td align="right">Iter</td><td>cc_dllst_iter_decr (Iter);</td>
	<td></td>
</tr>
<tr>
	
	<td rowspan="2">traversal</td><td align="right"></td><td>cc_dllst_trav (Object, Iter) { ... }</td>
	<td></td>
</tr>
<tr>
	
	<td align="right"></td><td>cc_dllst_trav_rev (Object, Iter) { ... }</td>
	<td></td>
</tr>
<tr>
	
	<td rowspan="2">capacity</td><td align="right">int</td><td>cc_dllst_size (Object);</td>
	<td></td>
</tr>
<tr>
	
	<td align="right">bool</td><td>cc_dllst_empty (Object);</td>
	<td></td>
</tr>
<tr>
	
	<td rowspan="2">modifiers</td><td align="right">void</td><td>cc_dllst_push_front (Object, Value);</td>
	<td></td>
</tr>
<tr>
	
	<td align="right">void</td><td>cc_dllst_push_back (Object, Value);</td>
	<td></td>
</tr>
</table>

## Motivation

Though C++ has existed for many years, there’s still no **efficient** and **instinctive** ways to create containers in C99 standard.

## Contributor

Kevin Dong Nai Jia <<kevin.dong.nai.jia@gmail.com>>

## License

This project is distributed under GPLv2 or later.
