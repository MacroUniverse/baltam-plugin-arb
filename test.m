load_plugin('arb');

%% 测试双精度特殊函数
val = 0.8151329368027908;
if abs((hypergeom(0.1, -0.2, 0.3) - val)/val) > eps
	error('failed!');
end

val = 0.8507621868425005 - 0.0701031742375557i;
if abs((hypergeom(0.1, -0.2+0.1i, 0.3) - val)/val) > eps
	error('failed!');
end

val = [0.8507621868425005 - 0.0701031742375557i,...
	   1.0372336913159646 + 0.0190169952845391i,...
	   0.9075294127990072 - 0.0443402804276223i];
if max(abs((hypergeom(0.1, -0.2+0.1i, [0.3,-0.1,0.2]) ...
	- val)./val)) > eps
	   error('failed!');
end

val = [0.8507621868425005 - 0.0701031742375557i
	   1.0372336913159646 + 0.0190169952845391i
	   0.9075294127990072 - 0.0443402804276223i];
if max(abs((hypergeom(0.1, -0.2+0.1i, [0.3;-0.1;0.2]) ...
	- val)./val)) > eps
	error('failed!');
end

val = 1.289458075329835;
if abs((gammaC(2.456) - val)/val) > eps
	 error('failed!');
end

val = [1.2894580753298350, 3.1184419980858190];
if max(abs((gammaC([2.456, -1.324]) - val)./val)) > 2*eps
	 error('failed!');
end

val = [1.289458075329835; ...
	 2.838662274628721 + 0.715540219254106i];
if max(abs((gammaC([2.456; -1.324+0.1i]) -...
	 val)./val)) > eps
	 error('failed!');
end

val = 0.25422203346472616;
if abs(lgamma(2.456) - val)/val > eps
	 error('failed!');
end

val = log([1.289458075329835
	 2.838662274628721 + 0.715540219254106i]);
if max((abs(lgamma([2.456; -1.324+0.1i]) -...
	 val)./val)) > 2*eps
	 error('failed!');
end

val = 4.5882301380287638e-286;
if abs((coulombF(203, -0.1, 6) - val)/val) > eps
	error('failed!');
end

val = [4.5882301380287638e-286
          -3.4792417308592423e-241-6.9067340830594833e-241i];
if max(abs((coulombF(203, -0.1, [6; 10-0.1i]) - val)./val)) > eps
	error('failed!');
end

%% 测试 BigInt 大整数
a = BigInt("1234567890223456789032345678904234567890");
b = BigInt("8765432109776543210967654321095765432109");
c = a + b;
% disp('是否等于 99999...9?');

unload_plugin('arb');
