import django_filters
from django.http import QueryDict
from django_filters import rest_framework as filters
from rest_framework import viewsets
from rest_framework.permissions import AllowAny
from rest_framework import filters as rest_filters
from django_filters import rest_framework as filters
from rest_framework.response import Response

from chat_app_service.serializer.users import UserSerializer
from chat_app_service.models import User


class UserFilter(django_filters.FilterSet):
    class Meta:
        model = User
        fields = ['username']


class UserViewSet(viewsets.ModelViewSet):
    queryset = User.objects.all().order_by('-created_at')
    permission_classes = (AllowAny,)
    serializer_class = UserSerializer
    filter_backends = (filters.DjangoFilterBackend, rest_filters.SearchFilter, rest_filters.OrderingFilter)
    filter_class = UserFilter

    def update(self, request, *args, **kwargs):
        password = request.query_params.get('password')
        status = request.query_params.get('status')
        QueryDict(mutable=True).copy()
        data = request.data
        if password:
            data['password'] = password
        if status:
            data['status'] = bool(int(status))

        partial = kwargs.pop('partial', False)
        instance = self.get_object()
        serializer = self.get_serializer(instance, data=request.data, partial=partial)
        serializer.is_valid(raise_exception=True)
        self.perform_update(serializer)

        if getattr(instance, '_prefetched_objects_cache', None):
            # If 'prefetch_related' has been applied to a queryset, we need to
            # forcibly invalidate the prefetch cache on the instance.
            instance._prefetched_objects_cache = {}

        return Response(serializer.data)

